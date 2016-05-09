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

#include <memory>
#include "Api.h"
#include "interface/oplkinc.h"
#include "MsgPtr.h"
#include "ReturnMessage_m.h"
#include "EventMessage_m.h"
#include "InitMessage_m.h"
#include "NmtMessage_m.h"
#include "ObdCbMessage_m.h"
#include "LinkMessage_m.h"
#include "LinkReturnMessage_m.h"
#include "ObjectMessage_m.h"
#include "ObjectReturnMessage_m.h"
#include "ObdAlConnectionMessage_m.h"
#include "ObdAlConnectionReturnMessage_m.h"
#include "BoolMessage_m.h"
#include "SdoMessage_m.h"
#include "LocalObjectMessage_m.h"
#include "LocalObjectReturnMessage_m.h"
#include "SendAsndFramePacket_m.h"
#include "AsndFramePacket_m.h"

using namespace std;
USING_NAMESPACE
Define_Module(Api);

void Api::initialize()
{
    interface::api::ApiFunctions & functions = mApi;

    // init interface
    interface::OplkApi::getInstance().initModule(&functions);

    // resolve return gate
    mReturnGate = gate("return");
    if (mReturnGate == nullptr)
        error("%s - unable to resolve gate", __PRETTY_FUNCTION__);
    mEventGate = gate("event");
    if (mReturnGate == nullptr)
        error("%s - unable to resolve gate", __PRETTY_FUNCTION__);
}

void Api::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        switch (static_cast<ApiCallType>(msg->getKind()))
        {
            case ApiCallType::init:
                sendReturnValue(mApi.initialize());
                break;
            case ApiCallType::create: {
                // cast message
                auto initMsg = dynamic_cast<oplkMessages::InitMessage*>(msg.get());

                if (initMsg != nullptr)
                {
                    auto initParam = initMsg->getInitParam();

                    // set event callback to static method
                    initParam.pfnCbEvent = processEvent;
                    // set event callback user arg to this pointer
                    initParam.pEventUserArg = static_cast<void*>(this);

                    sendReturnValue(mApi.create(&initParam));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::destroy:
                sendReturnValue(mApi.destroy());
                break;
            case ApiCallType::exit:
                mApi.exit();
                break;
            case ApiCallType::execNmtCommand: {
                // cast message
                auto nmtMsg = dynamic_cast<oplkMessages::NmtMessage*>(msg.get());

                if (nmtMsg != nullptr)
                    sendReturnValue(mApi.execNmtCommand(nmtMsg->getNmtEvent()));
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::cbGenericObdAccess: {
                // cast message
                auto obdMessage = dynamic_cast<oplkMessages::ObdCbMessage*>(msg.get());

                if (obdMessage != nullptr)
                    sendReturnValue(mApi.cbGenericObdAccess(&obdMessage->getObdCbParam()));
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::linkObject: {
                // cast message
                auto linkMsg = dynamic_cast<oplkMessages::LinkMessage*>(msg.get());

                if (linkMsg != nullptr)
                {
                    UINT varEntries = linkMsg->getVarEntries();
                    interface::api::ObdSize entrySize = linkMsg->getEntrySize();

                    auto ret = mApi.linkObject(linkMsg->getObjIndex(), (void*) linkMsg->getVariable(), &varEntries,
                            &entrySize, linkMsg->getFirstSubIndex());
                    auto retMsg = new oplkMessages::LinkReturnMessage();
                    retMsg->setReturnValue(ret);
                    retMsg->setVarEntries(varEntries);
                    retMsg->setEntrySize(entrySize);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::readObject: {
                // cast message
                auto objMsg = dynamic_cast<oplkMessages::ObjectMessage*>(msg.get());

                if (objMsg != nullptr)
                {
                    UINT size = objMsg->getObjDataArraySize();
                    std::unique_ptr<BYTE[]> destData(new BYTE[size] { 0 });

                    auto ret = mApi.readObject(&objMsg->getSdoComConHdl(), objMsg->getNodeId(), objMsg->getIndex(),
                            objMsg->getSubIndex(), destData.get(), &size, objMsg->getSdoType(),
                            (void*) objMsg->getUserArg());

                    // create return message and copy data from recieved message
                    auto retMsg = new oplkMessages::ObjectReturnMessage();
                    retMsg->setSdoComConHdl(objMsg->getSdoComConHdl());
                    retMsg->setNodeId(objMsg->getNodeId());
                    retMsg->setIndex(objMsg->getIndex());
                    retMsg->setSubIndex(objMsg->getSubIndex());
                    retMsg->setSdoType(objMsg->getSdoType());
                    retMsg->setUserArg(objMsg->getUserArg());

                    // allocate array for read data
                    objMsg->setObjDataArraySize(size);
                    // write read data to array
                    for (auto i = 0u; i < size; i++)
                        objMsg->setObjData(i, destData[i]);

                    // set return value
                    retMsg->setReturnValue(ret);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::writeObject: {
                // cast message
                auto objMsg = dynamic_cast<oplkMessages::ObjectMessage*>(msg.get());

                if (objMsg != nullptr)
                {
                    UINT size = objMsg->getObjDataArraySize();
                    std::unique_ptr<BYTE[]> destData(new BYTE[size] { 0 });

                    // copy transmitted data
                    for (auto i = 0u; i < size; i++)
                        destData[i] = objMsg->getObjData(i);

                    auto ret = mApi.writeObject(&objMsg->getSdoComConHdl(), objMsg->getNodeId(), objMsg->getIndex(),
                            objMsg->getSubIndex(), destData.get(), size, objMsg->getSdoType(),
                            (void*) objMsg->getUserArg());

                    // create return message and copy data from recieved message
                    auto retMsg = new oplkMessages::ObjectReturnMessage();
                    retMsg->setSdoComConHdl(objMsg->getSdoComConHdl());
                    retMsg->setNodeId(objMsg->getNodeId());
                    retMsg->setIndex(objMsg->getIndex());
                    retMsg->setSubIndex(objMsg->getSubIndex());
                    retMsg->setSdoType(objMsg->getSdoType());
                    retMsg->setUserArg(objMsg->getUserArg());

                    // set return value
                    retMsg->setReturnValue(ret);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::finishUserObdAccess: {
                // cast message
                auto connMsg = dynamic_cast<oplkMessages::ObdAlConnectionMessage*>(msg.get());

                if (connMsg != nullptr)
                {
                    UINT size = connMsg->getDataArraySize();
                    std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });

                    // copy transmitted data
                    for (auto i = 0u; i < size; i++)
                        data[i] = connMsg->getData(i);

                    auto connHdl = connMsg->getObdAlConnHdl();
                    connHdl.pDstData = data.get();
                    connHdl.pSrcData = data.get();

                    auto ret = mApi.finishUserObdAccess(&connHdl);

                    // create return message
                    auto retMsg = new oplkMessages::ObdAlConnectionReturnMessage();
                    retMsg->setDataArraySize(size);

                    // copy data
                    for (auto i = 0u; i < size; i++)
                        connMsg->setData(i, data[i]);

                    retMsg->setObdAlConnHdl(connHdl);
                    retMsg->setReturnValue(ret);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::enableUserObdAccess: {
                // cast message
                auto enableMsg = dynamic_cast<oplkMessages::BoolMessage*>(msg.get());

                if (enableMsg != nullptr)
                {
                    sendReturnValue(mApi.enableUserObdAccess(enableMsg->getValue()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::freeSdoChannel: {
                // cast message
                auto sdoMsg = dynamic_cast<oplkMessages::SdoMessage*>(msg.get());

                if (sdoMsg != nullptr)
                {
                    sendReturnValue(mApi.freeSdoChannel(sdoMsg->getComConHdl()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::abortSdoChannel: {
                // cast message
                auto sdoMsg = dynamic_cast<oplkMessages::SdoMessage*>(msg.get());

                if (sdoMsg != nullptr)
                {
                    sendReturnValue(mApi.abortSdoChannel(sdoMsg->getComConHdl(), sdoMsg->getAbortCode()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::readLocalObject: {
                // cast message
                auto objMsg = dynamic_cast<oplkMessages::LocalObjectMessage*>(msg.get());

                if (objMsg != nullptr)
                {
                    UINT size = objMsg->getDataArraySize();
                    std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });

                    auto ret = mApi.readLocalObject(objMsg->getIndex(), objMsg->getSubIndex(), data.get(), &size);

                    // create return message
                    auto retMsg = new oplkMessages::LocalObjectReturnMessage();

                    retMsg->setDataArraySize(size);
                    // copy data
                    for (auto i = 0u; i < size; i++)
                        retMsg->setData(i, data[i]);
                    retMsg->setIndex(objMsg->getIndex());
                    retMsg->setSubIndex(objMsg->getSubIndex());
                    retMsg->setReturnValue(ret);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::writeLocalObject: {
                // cast message
                auto objMsg = dynamic_cast<oplkMessages::LocalObjectMessage*>(msg.get());

                if (objMsg != nullptr)
                {
                    UINT size = objMsg->getDataArraySize();
                    std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });
                    // copy data
                    for (auto i = 0u; i < size; i++)
                        data[i] = objMsg->getData(i);

                    auto ret = mApi.writeLocalObject(objMsg->getIndex(), objMsg->getSubIndex(), data.get(), size);

                    // create return message
                    auto retMsg = new oplkMessages::LocalObjectReturnMessage();

                    retMsg->setDataArraySize(size);
                    // copy data
                    for (auto i = 0u; i < size; i++)
                        retMsg->setData(i, data[i]);
                    retMsg->setIndex(objMsg->getIndex());
                    retMsg->setSubIndex(objMsg->getSubIndex());
                    retMsg->setReturnValue(ret);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::sendAsndFrame: {
                // cast message
                auto framePkt = dynamic_cast<oplkMessages::SendAsndFramePacket*>(msg.get());

                if (framePkt != nullptr)
                {
                    auto frame = dynamic_cast<oplkMessages::AsndFramePacket*>(framePkt->getEncapsulatedPacket());

                    if (frame != nullptr)
                    {
                        interface::api::AsndFrame frameObj;
                        frameObj.serviceId = frame->getServiceId();
                        auto packet = frame->getEncapsulatedPacket();
                        switch(frameObj.serviceId)
                        {
                            case 0: //TODO: implement specific casting and obj initialization
                                break;
                            default:
                                break;
                        }

                        sendReturnValue(mApi.sendAsndFrame(framePkt->getDestNodeId(), &frameObj, framePkt->getAsndSize()));
                    }
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
            }
                break;
            case ApiCallType::sendEthFrame:
                break;
            case ApiCallType::setAsndForward:
                break;
            case ApiCallType::setNonPlkForward:
                break;
            case ApiCallType::postUserEvent:
                break;
            case ApiCallType::triggerMnStateChange:
                break;
            case ApiCallType::setCdcBuffer:
                break;
            case ApiCallType::setOdArchivePath:
                break;
            case ApiCallType::setCdcFilename:
                break;
            case ApiCallType::process:
                break;
            case ApiCallType::getIdentResponse:
                break;
            case ApiCallType::getEthMacAddr:
                break;
            case ApiCallType::checkKernelStack:
                break;
            case ApiCallType::waitSyncEvent:
                break;
            case ApiCallType::getVersion:
                break;
            case ApiCallType::getVersionString:
                break;
            case ApiCallType::getStackConfiguration:
                break;
            case ApiCallType::getStackInfo:
                break;
            case ApiCallType::getSocTime:
                break;

            case ApiCallType::allocProcessImage:
                break;
            case ApiCallType::freeProcessImage:
                break;
            case ApiCallType::linkProcessImageObject:
                break;
            case ApiCallType::exchangeProcessImageIn:
                break;
            case ApiCallType::exchangeProcessImageOut:
                break;
            case ApiCallType::getProcessImageIn:
                break;
            case ApiCallType::getProcessImageOut:
                break;
            case ApiCallType::setupProcessImage:
                break;

            case ApiCallType::triggerPresForward:
                break;

            case ApiCallType::getRetStr:
                break;

            default:
                error("%s - unknown message kind received %d", __PRETTY_FUNCTION__, msg->getKind());
        }
    }
}

interface::api::ErrorType Api::processEvent(interface::api::ApiEventType eventType_p,
        interface::api::ApiEventArg* pEventArg_p)
{
    // create event message
    auto eventMsg = new oplkMessages::EventMessage();
    eventMsg->setEventType(eventType_p);
    eventMsg->setEventArg(*pEventArg_p);

    // send message
    send(eventMsg, mEventGate);

    // wait for return value
    std::unique_ptr<oplkMessages::ReturnMessage> returnMsg/*(TODO receive message)*/;

    return interface::api::Error::kErrorOk; //returnMsg->getReturnValue();
}

interface::api::ErrorType Api::processEvent(interface::api::ApiEventType eventType_p,
        interface::api::ApiEventArg* pEventArg_p, void* pUserArg_p)
{
    // cast user arg
    auto api = reinterpret_cast<Api*>(pUserArg_p);

    // call api method
    return api->processEvent(eventType_p, pEventArg_p);
}

void Api::sendReturnValue(interface::api::ErrorType returnValue)
{
    auto retMsg = new oplkMessages::ReturnMessage();
    retMsg->setReturnValue(returnValue);
    sendReturnMessage(retMsg);
}

void Api::sendReturnMessage(cMessage* msg)
{
    send(msg, mReturnGate);
}
