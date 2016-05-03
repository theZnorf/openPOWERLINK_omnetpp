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
        interface::api::ErrorType ret = interface::api::Error::kErrorOk;

        switch (static_cast<ApiCallType>(msg->getKind())) {
            case ApiCallType::init:
                ret = mApi.initialize();
                break;
            case ApiCallType::create:
            {
                // cast message
                auto initMsg = dynamic_cast<oplkMessages::InitMessage*>(msg.get());

                if (initMsg != nullptr)
                {
                    auto initParam = initMsg->getInitParam();

                    // set event callback to static method
                    initParam.pfnCbEvent = processEvent;
                    // set event callback user arg to this pointer
                    initParam.pEventUserArg = static_cast<void*>(this);

                    ret = mApi.create(&initParam);
                }
                break;
            }
            case ApiCallType::destroy:
                break;
            case ApiCallType::exit:
                break;
            case ApiCallType::execNmtCommand:
                break;
            case ApiCallType::cbGenericObdAccess:
                break;
            case ApiCallType::linkObject:
                break;
            case ApiCallType::readObject:
                break;
            case ApiCallType::writeObject:
                break;
            case ApiCallType::finishUserObdAccess:
                break;
            case ApiCallType::enableUserObdAccess:
                break;
            case ApiCallType::freeSdoChannel:
                break;
            case ApiCallType::abortSdoChannel:
                break;
            case ApiCallType::readLocalObject:
                break;
            case ApiCallType::writeLocalObject:
                break;
            case ApiCallType::sendAsndFrame:
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

        sendReturnMessage(ret);
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

void Api::sendReturnMessage(interface::api::ErrorType returnValue)
{
    auto retMsg = new oplkMessages::ReturnMessage();
    retMsg->setReturnValue(returnValue);

    send(retMsg, mReturnGate);
}
