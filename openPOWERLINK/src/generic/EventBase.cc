/**
 ********************************************************************************
 \file   EventBase.cc

 \brief  Implementation of Base class of Event modules.

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#include "EventBase.h"
#include "ApiMessages.h"
#include "MsgPtr.h"
#include "debugstr.h"
#include "OplkException.h"

using namespace std;
USING_NAMESPACE

Define_Module(EventBase);

void EventBase::initialize()
{
    // resolve gates
    //mReturnGate = gate("eventReturn");
    mShutdownGate = gate("stackShutdown");

    // register signals
    mEventTypeSignal = registerSignal("eventType");
    mNmtStateSignal = registerSignal("nmtState");

    // resolve parameter
    mPrintEventType = par("printEventType");
}

void EventBase::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        // cast to EventMessage
        auto eventMsg = dynamic_cast<oplkMessages::EventMessage*>(msg.get());

        if (eventMsg != nullptr)
        {
            // process Event
            //auto ret = processEvent(eventMsg->getEventType(), eventMsg->getEventArg());
            processEvent(eventMsg->getEventType(), eventMsg->getEventArg(),
                    (void*) eventMsg->getUserArg());

            // create event return message
//            auto retMsg = new oplkMessages::EventReturnMessage();
//            retMsg->setName(("return - " + std::string(msg->getName())).c_str());
//            retMsg->setReturnValue(ret);
//            retMsg->setEventType(eventMsg->getEventType());
//            retMsg->setEventArg(eventMsg->getEventArg());
//            retMsg->setUserArg(eventMsg->getUserArg());
//
//            send(retMsg, mReturnGate);
        }
    }
}

interface::api::ErrorType EventBase::processEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    if (mPrintEventType)
    {
        EV << "Process event:" << endl;
        EV << " EventType: " << interface::debug::getApiEventStr(eventType)
                << endl;
    }

    emit(mEventTypeSignal, eventType);

    interface::api::ErrorType ret = interface::api::Error::kErrorOk;

    switch (eventType)
    {
        case interface::api::ApiEvent::kOplkApiEventNmtStateChange:
            emit(mNmtStateSignal, eventArg.nmtStateChange.newNmtState);
            ret = processNmtStateChangeEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventCriticalError:
            ret = processErrorEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventWarning:
            ret = processWarningEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventHistoryEntry:
            ret = processHistoryEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventNode:
            ret = processNodeEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventPdoChange:
            ret = processPdoChangeEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventCfmProgress:
            ret = processCfmProgressEvent(eventType, eventArg, userArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventCfmResult:
            ret = processCfmResultEvent(eventType, eventArg, userArg);
            break;

        default:
            break;
    }
    return ret;
}

interface::api::ErrorType EventBase::processNmtStateChangeEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    EV << "NMT State change from "
            << interface::debug::getNmtStateStr(
                    eventArg.nmtStateChange.oldNmtState) << " to "
            << interface::debug::getNmtStateStr(
                    eventArg.nmtStateChange.newNmtState) << endl;

    // check shutdown
    if (eventArg.nmtStateChange.newNmtState
            == interface::api::NmtStateE::kNmtGsOff)
    {
        EV << "Stack received kNmtGsOff!" << endl;
        send(new cMessage("shutdown demo"), mShutdownGate);
    }

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processErrorEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    EV << "Error event: ";
    EV << "  Error:       "
            << interface::debug::getRetValStr(eventArg.internalError.oplkError)
            << endl;
    EV << "  EventSource: "
            << interface::debug::getEventSourceStr(
                    eventArg.internalError.eventSource) << endl;
    EV << "  ErrorArg:    " << eventArg.internalError.errorArg.uintArg << endl;

    send(new cMessage("shutdown demo"), mShutdownGate);
    //error(interface::debug::getRetValStr(eventArg.internalError.oplkError));

    //throw interface::OplkException("ErrorEvent", eventArg.internalError.oplkError);

    return eventArg.internalError.oplkError;
}

interface::api::ErrorType EventBase::processWarningEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    EV << "Warning event: "
            << interface::debug::getRetValStr(eventArg.internalError.oplkError)
            << endl;

    return eventArg.internalError.oplkError;
}

interface::api::ErrorType EventBase::processHistoryEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    auto event = eventArg.errorHistoryEntry;

    EV << std::hex << "History event: " << endl;
    EV << "   Type:      " << event.entryType << endl;
    EV << "   ErrorCode: " << event.errorCode << " - "
            << interface::debug::getRetValStr(event.errorCode) << endl;
    EV << "   Timestamp: " << event.timeStamp.sec << " - "
            << event.timeStamp.nsec << endl;
    EV << "   Addr:      ";
    for (auto i = 0u; i < sizeof(event.aAddInfo); i++)
        EV << " " << static_cast<unsigned int>(event.aAddInfo[i]);
    EV << endl << std::dec;

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processNodeEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    auto event = eventArg.nodeEvent;

    EV << "Node event: " << endl;
    EV << "   Type:      "
            << interface::debug::getNmtNodeEventTypeStr(event.nodeEvent)
            << endl;
    EV << "   ErrorCode: " << event.errorCode << endl;
    EV << "   Mandatory: " << event.fMandatory << endl;
    EV << "   NmtState:  " << interface::debug::getNmtStateStr(event.nmtState)
            << endl;
    EV << "   NodeId:    " << event.nodeId << endl;

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processPdoChangeEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    auto event = eventArg.pdoChange;

    EV << "PDO change event: " << endl;
    EV << "   NodeId:    " << event.nodeId << endl;
    EV << "   ParamIdx:  " << event.mappParamIndex << endl;
    EV << "   ObjCount:  " << event.mappObjectCount << endl;
    EV << "   Activated: " << event.fActivated << endl;
    EV << "   Tx:        " << event.fTx << endl;

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processCfmProgressEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    auto event = eventArg.cfmProgress;

    EV << "Cfm progress event: " << endl;
    EV << "   NodeId:          " << event.nodeId << endl;
    EV << "   BytesDownloaded: " << event.bytesDownloaded << endl;
    EV << "   Error:           " << event.error << " - "
            << interface::debug::getRetValStr(event.error) << endl;
    EV << "   ObjectIndex:     " << event.objectIndex << endl;
    EV << "   ObjectSubIndex:  " << event.objectSubIndex << endl;
    EV << "   SdoAbortCode:    " << event.sdoAbortCode << " - "
            << interface::debug::getAbortCodeStr(event.sdoAbortCode) << endl;
    EV << "   TotalNumOfBytes: " << event.totalNumberOfBytes << endl;

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processCfmResultEvent(
        interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg, void* userArg)
{
    auto event = eventArg.cfmResult;

    EV << "Cfm progress event: " << endl;
    EV << "   NodeId:         " << event.nodeId << endl;
    EV << "   NmtNodeCommand: "
            << interface::debug::getNmtNodeCommandTypeStr(event.nodeCommand)
            << endl;

    return interface::api::Error::kErrorOk;
}

