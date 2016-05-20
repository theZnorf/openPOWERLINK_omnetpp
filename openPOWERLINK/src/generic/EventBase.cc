//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

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
    mReturnGate = gate("eventReturn");
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
            auto ret = processEvent(eventMsg->getEventType(), eventMsg->getEventArg());

            // create event return message
            auto retMsg = new oplkMessages::EventReturnMessage();
            retMsg->setName(("return - " + std::string(msg->getName())).c_str());
            retMsg->setReturnValue(ret);
            retMsg->setEventType(eventMsg->getEventType());
            retMsg->setEventArg(eventMsg->getEventArg());

            send(retMsg, mReturnGate);
        }
    }
}

interface::api::ErrorType EventBase::processEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg)
{
    if (mPrintEventType)
    {
        EV << "Process event:" << endl;
        EV << " EventType: " << interface::debug::getApiEventStr(eventType) << endl;
    }

    emit(mEventTypeSignal, eventType);

    interface::api::ErrorType ret = interface::api::Error::kErrorOk;

    switch (eventType)
    {
        case interface::api::ApiEvent::kOplkApiEventNmtStateChange:
            emit(mNmtStateSignal, eventArg.nmtStateChange.newNmtState);
            ret = processNmtStateChangeEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventCriticalError:
            ret = processErrorEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventWarning:
            ret = processWarningEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventHistoryEntry:
            ret = processHistoryEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventNode:
            ret = processNodeEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventPdoChange:
            ret = processPdoChangeEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventCfmProgress:
            ret = processCfmProgressEvent(eventType, eventArg);
            break;

        case interface::api::ApiEvent::kOplkApiEventCfmResult:
            ret = processCfmResultEvent(eventType, eventArg);
            break;

        default:
            break;
    }
    return ret;
}

interface::api::ErrorType EventBase::processNmtStateChangeEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{
    EV << "NMT State change from " << interface::debug::getNmtStateStr(eventArg.nmtStateChange.oldNmtState) << " to " << interface::debug::getNmtStateStr(eventArg.nmtStateChange.newNmtState) << endl;

    // check shutdown
    if (eventArg.nmtStateChange.newNmtState == interface::api::NmtStateE::kNmtGsOff)
    {
        EV << "Stack received kNmtGsOff!" << endl;
        send(new cMessage(), mShutdownGate);
    }

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processErrorEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{
    EV << "Error event: " ;
    EV << "  Error:       "<< interface::debug::getRetValStr(eventArg.internalError.oplkError) << endl;
    EV << "  EventSource: "<< interface::debug::getEventSourceStr(eventArg.internalError.eventSource) << endl;
    EV << "  ErrorArg:    "<< eventArg.internalError.errorArg.uintArg << endl;

    error(interface::debug::getRetValStr(eventArg.internalError.oplkError));

    throw interface::OplkException("ErrorEvent", eventArg.internalError.oplkError);

    return eventArg.internalError.oplkError;
}

interface::api::ErrorType EventBase::processWarningEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{
    EV << "Warning event: " << interface::debug::getRetValStr(eventArg.internalError.oplkError) << endl;

    return eventArg.internalError.oplkError;
}

interface::api::ErrorType EventBase::processHistoryEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processNodeEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processPdoChangeEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processCfmProgressEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{

    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType EventBase::processCfmResultEvent(interface::api::ApiEventType eventType,
        interface::api::ApiEventArg eventArg)
{

    return interface::api::Error::kErrorOk;
}

