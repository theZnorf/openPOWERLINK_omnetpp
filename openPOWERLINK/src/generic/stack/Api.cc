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
#include <algorithm>
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
#include "SendPlkPacket_m.h"
#include "PlkPacket_m.h"
#include "AsndFilterMessage_m.h"
#include "PointerContMessage_m.h"
#include "NmtNodeMessage_m.h"
#include "StringMessage_m.h"
#include "UintMessage_m.h"
#include "ReturnPacket_m.h"
#include "IdentResponsePacket_m.h"
#include "MacReturnMessage_m.h"
#include "UlongMessage_m.h"
#include "StackInfoReturnMessage_m.h"
#include "SocTimeReturnMessage_m.h"
#include "ProcessImageSIzeMessage_m.h"
#include "LinkProcessImageMessage_m.h"
#include "LinkProcessImageReturnMessage_m.h"

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
                    auto retMsg = new oplkMessages::LinkReturnMessage("LinkObjectReturn", msg->getKind());
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
                    auto retMsg = new oplkMessages::ObjectReturnMessage("ReadObjectReturn", msg->getKind());
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
                    auto retMsg = new oplkMessages::ObjectReturnMessage("WriteObjectReturn", msg->getKind());
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
                    auto retMsg = new oplkMessages::ObdAlConnectionReturnMessage("ObdAlConnectionReturn",
                            msg->getKind());
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
                break;
            }
            case ApiCallType::freeSdoChannel: {
                // cast message
                auto sdoMsg = dynamic_cast<oplkMessages::SdoMessage*>(msg.get());

                if (sdoMsg != nullptr)
                {
                    sendReturnValue(mApi.freeSdoChannel(sdoMsg->getComConHdl()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::abortSdoChannel: {
                // cast message
                auto sdoMsg = dynamic_cast<oplkMessages::SdoMessage*>(msg.get());

                if (sdoMsg != nullptr)
                {
                    sendReturnValue(mApi.abortSdoChannel(sdoMsg->getComConHdl(), sdoMsg->getAbortCode()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::readLocalObject: {
                // cast message
                auto objMsg = dynamic_cast<oplkMessages::LocalObjectMessage*>(msg.get());

                if (objMsg != nullptr)
                {
                    UINT size = objMsg->getDataArraySize();
                    std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });

                    auto ret = mApi.readLocalObject(objMsg->getIndex(), objMsg->getSubIndex(), data.get(), &size);

                    // create return message
                    auto retMsg = new oplkMessages::LocalObjectReturnMessage("ReadLocalObjectReturn", msg->getKind());

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
                break;
            }
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
                    auto retMsg = new oplkMessages::LocalObjectReturnMessage("WriteLocalObjectReturn", msg->getKind());

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
                break;
            }
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
                        // copy payload
                        for (auto i = 0u; i < frame->getPayLoadArraySize(); i++)
                            frameObj.payload.aPayload[i] = frame->getPayLoad(i);

                        sendReturnValue(
                                mApi.sendAsndFrame(framePkt->getDestNodeId(), &frameObj, framePkt->getAsndSize()));
                    }
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::sendEthFrame: {
                // cast message
                auto plkPkt = dynamic_cast<oplkMessages::SendPlkPacket*>(msg.get());

                if (plkPkt != nullptr)
                {
                    auto frame = dynamic_cast<oplkMessages::PlkPacket*>(plkPkt->getEncapsulatedPacket());

                    if (frame != nullptr)
                    {
                        interface::api::PlkFrame frameObj;

                        // copy mac addresses
                        for (auto i = 0u; i < frame->getDstMacArraySize(); i++)
                        {
                            frameObj.aDstMac[i] = frame->getDstMac(i);
                            frameObj.aSrcMac[i] = frame->getSrcMac(i);
                        }
                        frameObj.etherType = frame->getEtherType();
                        frameObj.messageType = frame->getMsgType();
                        frameObj.dstNodeId = frame->getDstNodeId();
                        frameObj.srcNodeId = frame->getSrcNodeId();

                        auto ptr = &frameObj.data.asnd.serviceId;

                        // copy payload
                        for (auto i = 0u; i < frame->getPayLoadArraySize(); i++)
                            ptr[i] = frame->getPayLoad(i);

                        sendReturnValue(mApi.sendEthFrame(&frameObj, plkPkt->getFrameSize()));
                    }
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::setAsndForward: {
                // cast message
                auto filterMsg = dynamic_cast<oplkMessages::AsndFilterMessage*>(msg.get());

                if (filterMsg != nullptr)
                {
                    sendReturnValue(mApi.setAsndForward(filterMsg->getServiceId(), filterMsg->getFilterType()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::setNonPlkForward: {
                // cast message
                auto fwdMsg = dynamic_cast<oplkMessages::BoolMessage*>(msg.get());

                if (fwdMsg != nullptr)
                {
                    sendReturnValue(mApi.setNonPlkForward(fwdMsg->getValue()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::postUserEvent: {
                // cast message
                auto eventMsg = dynamic_cast<oplkMessages::PointerContMessage*>(msg.get());

                if (eventMsg != nullptr)
                {
                    sendReturnValue(mApi.postUserEvent((void*) eventMsg->getPointer()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::triggerMnStateChange: {
                // cast message
                auto nmtMsg = dynamic_cast<oplkMessages::NmtNodeMessage*>(msg.get());

                if (nmtMsg != nullptr)
                {
                    sendReturnValue(mApi.triggerMnStateChange(nmtMsg->getNodeId(), nmtMsg->getNodeCommand()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::setCdcBuffer:
                // TODO: check and implement
                break;
            case ApiCallType::setOdArchivePath: {
                // cast message
                auto archiveMsg = dynamic_cast<oplkMessages::StringMessage*>(msg.get());

                if (archiveMsg != nullptr)
                {
                    sendReturnValue(mApi.setOdArchivePath(archiveMsg->getString()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::setCdcFilename: {
                // cast message
                auto cdcMsg = dynamic_cast<oplkMessages::StringMessage*>(msg.get());

                if (cdcMsg != nullptr)
                {
                    sendReturnValue(mApi.setCdcFilename(cdcMsg->getString()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::process:
                sendReturnValue(mApi.process());
                break;
            case ApiCallType::getIdentResponse: {
                // cast message
                auto nodeMsg = dynamic_cast<oplkMessages::UintMessage*>(msg.get());

                if (nodeMsg != nullptr)
                {
                    interface::api::IdentResponse* identResponse;

                    auto ret = mApi.getIdentResponse(nodeMsg->getValue(), &identResponse);

                    // create returning packet with encapsulated identresponse
                    auto identPkt = new oplkMessages::IdentResponsePacket();
                    identPkt->setFlag1(identResponse->flag1);
                    identPkt->setFlag2(identResponse->flag2);
                    identPkt->setNmtStatus(identResponse->nmtStatus);
                    identPkt->setIdentResponseFlags(identResponse->identResponseFlags);
                    identPkt->setPowerlinkProfileVersion(identResponse->powerlinkProfileVersion);
                    identPkt->setReserved1(identResponse->reserved1);
                    identPkt->setFeatureFlagsLe(identResponse->featureFlagsLe);
                    identPkt->setMtuLe(identResponse->mtuLe);
                    identPkt->setPollInSizeLe(identResponse->pollInSizeLe);
                    identPkt->setPollOutSizeLe(identResponse->pollOutSizeLe);
                    identPkt->setResponseTimeLe(identResponse->responseTimeLe);
                    identPkt->setReserved2(identResponse->reserved2);
                    identPkt->setDeviceTypeLe(identResponse->deviceTypeLe);
                    identPkt->setVendorIdLe(identResponse->vendorIdLe);
                    identPkt->setProductCodeLe(identResponse->productCodeLe);
                    identPkt->setRevisionNumberLe(identResponse->revisionNumberLe);
                    identPkt->setSerialNumberLe(identResponse->serialNumberLe);
                    identPkt->setVendorIdLe(identResponse->vendorSpecificExt1Le);
                    identPkt->setVerifyConfigurationDateLe(identResponse->verifyConfigurationDateLe);
                    identPkt->setVerifyConfigurationTimeLe(identResponse->verifyConfigurationTimeLe);
                    identPkt->setApplicationSwDateLe(identResponse->applicationSwDateLe);
                    identPkt->setApplicationSwTimeLe(identResponse->applicationSwTimeLe);
                    identPkt->setIpAddressLe(identResponse->ipAddressLe);
                    identPkt->setSubnetMaskLe(identResponse->subnetMaskLe);
                    identPkt->setDefaultGatewayLe(identResponse->defaultGatewayLe);

                    // Hostname
                    for (auto i = 0u; i < sizeof(identResponse->sHostName); i++)
                        identPkt->setHostName(i, identResponse->sHostName[i]);

                    // vendor specific ext 2
                    for (auto i = 0u; i < sizeof(identResponse->aVendorSpecificExt2); i++)
                        identPkt->setVendorSpecificExt2(i, identResponse->aVendorSpecificExt2[i]);

                    auto retPkt = new oplkMessages::ReturnPacket("GetIdentResponseReturn", msg->getKind());
                    retPkt->setReturnValue(ret);
                    retPkt->encapsulate(identPkt);

                    sendReturnMessage(retPkt);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::getEthMacAddr: {
                BYTE mac[6];
                auto ret = mApi.getEthMacAddr(mac);

                auto macMsg = new oplkMessages::MacReturnMessage("GetMacReturn", msg->getKind());

                macMsg->setReturnValue(ret);
                for (auto i = 0u; i < sizeof(mac); i++)
                    macMsg->setMac(i, mac[i]);

                sendReturnMessage(macMsg);
                break;
            }
            case ApiCallType::checkKernelStack: {
                auto ret = mApi.checkKernelStack();

                auto checkMsg = new oplkMessages::BoolMessage("CheckKernelStackReturn", msg->getKind());
                checkMsg->setValue(ret);

                sendReturnMessage(checkMsg);
                break;
            }
            case ApiCallType::waitSyncEvent: {
                // cast message
                auto timeoutMsg = dynamic_cast<oplkMessages::UlongMessage*>(msg.get());

                if (timeoutMsg != nullptr)
                {
                    sendReturnValue(mApi.waitSyncEvent(timeoutMsg->getValue()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::getVersion: {
                auto ret = mApi.getVersion();

                auto versionMsg = new oplkMessages::UintMessage("GetVersionReturn", msg->getKind());

                versionMsg->setValue(ret);

                sendReturnMessage(versionMsg);
                break;
            }
            case ApiCallType::getVersionString: {
                auto ret = mApi.getVersionString();

                auto versionMsg = new oplkMessages::StringMessage("GetVersionStringReturn", msg->getKind());

                versionMsg->setString(ret);

                sendReturnMessage(versionMsg);
                break;
            }
            case ApiCallType::getStackConfiguration: {
                auto ret = mApi.getStackConfiguration();

                auto confMsg = new oplkMessages::UintMessage("GetStackConfigurationReturn", msg->getKind());

                confMsg->setValue(ret);

                sendReturnMessage(confMsg);
                break;
            }
            case ApiCallType::getStackInfo: {
                interface::api::ApiStackInfo info;

                auto ret = mApi.getStackInfo(&info);

                auto infoMsg = new oplkMessages::StackInfoReturnMessage("GetStackInfoReturn", msg->getKind());

                infoMsg->setReturnValue(ret);
                infoMsg->setInfo(info);

                sendReturnMessage(infoMsg);
                break;
            }
            case ApiCallType::getSocTime: {
                interface::api::SocTimeInfo info;

                auto ret = mApi.getSocTime(&info);

                auto infoMsg = new oplkMessages::SocTimeReturnMessage("GetSocTimeReturn", msg->getKind());

                infoMsg->setReturnValue(ret);
                infoMsg->setInfo(info);

                sendReturnMessage(infoMsg);

                break;
            }

            case ApiCallType::allocProcessImage: {
                // cast message
                auto sizeMsg = dynamic_cast<oplkMessages::ProcessImageSIzeMessage*>(msg.get());

                if (sizeMsg != nullptr)
                {
                    sendReturnValue(mApi.allocProcessImage(sizeMsg->getSizeIn(), sizeMsg->getSizeOut()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::freeProcessImage:
                sendReturnValue(mApi.freeProcessImage());
                break;
            case ApiCallType::linkProcessImageObject: {
                // cast message
                auto linkMsg = dynamic_cast<oplkMessages::LinkProcessImageMessage*>(msg.get());

                if (linkMsg != nullptr)
                {
                    UINT varEntries = linkMsg->getVarEntries();
                    auto ret = mApi.linkProcessImageObject(linkMsg->getObjIndex(), linkMsg->getFirstSubIndex(),
                            linkMsg->getOffset(), linkMsg->getOutputPi(), linkMsg->getEntrySize(), &varEntries);

                    // create return message
                    auto retMsg = new oplkMessages::LinkProcessImageReturnMessage("LinkProcessImageReturn",
                            msg->getKind());
                    retMsg->setReturnValue(ret);
                    retMsg->setVarEntries(varEntries);

                    sendReturnMessage(retMsg);
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }
            case ApiCallType::exchangeProcessImageIn:
                sendReturnValue(mApi.exchangeProcessImageIn());
                break;
            case ApiCallType::exchangeProcessImageOut:
                sendReturnValue(mApi.exchangeProcessImageOut());
                break;
            case ApiCallType::getProcessImageIn: {
                auto img = mApi.exchangeProcessImageIn();

                auto retMsg = new oplkMessages::PointerContMessage("GetProcessImageInReturn", msg->getKind());
                retMsg->setPointer(img);

                sendReturnMessage(retMsg);
                break;
            }
            case ApiCallType::getProcessImageOut: {
                auto img = mApi.exchangeProcessImageOut();

                auto retMsg = new oplkMessages::PointerContMessage("GetProcessImageOutReturn", msg->getKind());
                retMsg->setPointer(img);

                sendReturnMessage(retMsg);
                break;
            }
            case ApiCallType::setupProcessImage:
                sendReturnValue(mApi.setupProcessImage());
                break;

            case ApiCallType::triggerPresForward: {
                // cast message
                auto nodeMsg = dynamic_cast<oplkMessages::UintMessage*>(msg.get());

                if (nodeMsg != nullptr)
                {
                    sendReturnValue(mApi.triggerPresForward(nodeMsg->getValue()));
                }
                else
                    sendReturnValue(interface::api::Error::kErrorInvalidOperation);
                break;
            }

            case ApiCallType::getRetStr:
                // TODO: check and implement
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
