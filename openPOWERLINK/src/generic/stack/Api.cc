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
#include "ApiMessages.h"
#include "debugstr.h"
#include "AppBase.h"
#include "OplkException.h"

using namespace std;
USING_NAMESPACE

Define_Module(Api);

constexpr const char* const Api::cApiCallNames[];

Api::Api() :
        cSimpleModule(32000), mReturnGate(nullptr), mEventGate(nullptr)
{
}

void Api::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkApi::Instance numberOfInstances = par("numberOfInstances");

    // init stub
    interface::OplkApi::setLibraryInfo(libName, numberOfInstances);
    interface::OplkApi::getInstance().initModule(this);

    // resolve and register return gates
    for (auto i = 0; i < gateSize("functionCallReturn"); i++)
    {
        mReturnGates.push_back(gate("functionCallReturn", i));
        mDispatcher.registerFunction(gate("functionCall", i),
                std::bind(&Api::handleApiCall, this, std::placeholders::_1));
    }

    // register signals
    mInvokedApiFunctionSignal = registerSignal("invokedFunctionType");
}

void Api::activity()
{
    while (true)
    {
        auto msg = receive();

        if (msg != nullptr)
            handleRawMessage(msg);
    }
}

void Api::handleRawMessage(cMessage* rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        //handleApiCall(msg);
        mDispatcher.dispatch(msg.get());
    }
}

void Api::handleApiCall(RawMessagePtr msg)
{
    interface::api::ErrorType ret =
            interface::api::Error::kErrorInvalidOperation;
    cMessage* retPtr = nullptr;

    emit(mInvokedApiFunctionSignal, msg->getKind());

    auto callType = static_cast<ApiCallType>(msg->getKind());

    switch (callType)
    {
        case ApiCallType::init:
            if (mApi.initialize != nullptr)
                ret = mApi.initialize();
            break;
        case ApiCallType::create: {
            // cast message
            auto initMsg = dynamic_cast<oplkMessages::InitMessage*>(msg);

            if ((initMsg != nullptr) && (mApi.create != nullptr))
            {
                auto initParam = initMsg->getInitParam();
                ret = mApi.create(&initParam);
            }
            break;
        }
        case ApiCallType::destroy:
            if (mApi.destroy != nullptr)
                ret = mApi.destroy();
            break;
        case ApiCallType::exit:
            if (mApi.exit != nullptr)
                mApi.exit();
            break;
        case ApiCallType::execNmtCommand: {
            // cast message
            auto nmtMsg = dynamic_cast<oplkMessages::NmtMessage*>(msg);

            if (nmtMsg != nullptr)
                ret = mApi.execNmtCommand(nmtMsg->getNmtEvent());
        }
            break;
        case ApiCallType::cbGenericObdAccess: {
            // cast message
            auto obdMessage = dynamic_cast<oplkMessages::ObdCbMessage*>(msg);

            if ((obdMessage != nullptr) && (mApi.cbGenericObdAccess != nullptr))
                ret = mApi.cbGenericObdAccess(&obdMessage->getObdCbParam());
        }
            break;
        case ApiCallType::linkObject: {
            // cast message
            auto linkMsg = dynamic_cast<oplkMessages::LinkMessage*>(msg);

            if ((linkMsg != nullptr) && (mApi.linkObject != nullptr))
            {
                UINT varEntries = linkMsg->getVarEntries();
                interface::api::ObdSize entrySize = linkMsg->getEntrySize();
                ret = mApi.linkObject(linkMsg->getObjIndex(),
                        (void*) linkMsg->getVariable(), &varEntries, &entrySize,
                        linkMsg->getFirstSubIndex());
                auto retMsg = new oplkMessages::LinkReturnMessage(
                        "LinkObjectReturn", msg->getKind());
                retMsg->setVarEntries(varEntries);
                retMsg->setEntrySize(entrySize);

                retPtr = retMsg;
            }
        }
            break;
        case ApiCallType::readObject: {
            // cast message
            auto objMsg = dynamic_cast<oplkMessages::ObjectMessage*>(msg);

            if (objMsg != nullptr)
            {
                UINT size = objMsg->getObjDataArraySize();
                std::unique_ptr<BYTE[]> destData(new BYTE[size] { 0 });

                if (mApi.readObject != nullptr)
                {
                    ret = mApi.readObject(&objMsg->getSdoComConHdl(),
                            objMsg->getNodeId(), objMsg->getIndex(),
                            objMsg->getSubIndex(), destData.get(), &size,
                            objMsg->getSdoType(), (void*) objMsg->getUserArg());

                    // create return message and copy data from recieved message
                    auto retMsg = new oplkMessages::ObjectReturnMessage(
                            "ReadObjectReturn", msg->getKind());
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

                    retPtr = retMsg;
                }
            }
        }
            break;
        case ApiCallType::writeObject: {
            // cast message
            auto objMsg = dynamic_cast<oplkMessages::ObjectMessage*>(msg);

            if ((objMsg != nullptr) && (mApi.writeObject != nullptr))
            {
                UINT size = objMsg->getObjDataArraySize();
                std::unique_ptr<BYTE[]> destData(new BYTE[size] { 0 });

                // copy transmitted data
                for (auto i = 0u; i < size; i++)
                    destData[i] = objMsg->getObjData(i);

                ret = mApi.writeObject(&objMsg->getSdoComConHdl(),
                        objMsg->getNodeId(), objMsg->getIndex(),
                        objMsg->getSubIndex(), destData.get(), size,
                        objMsg->getSdoType(), (void*) objMsg->getUserArg());

                // create return message and copy data from recieved message
                auto retMsg = new oplkMessages::ObjectReturnMessage(
                        "WriteObjectReturn", msg->getKind());
                retMsg->setSdoComConHdl(objMsg->getSdoComConHdl());
                retMsg->setNodeId(objMsg->getNodeId());
                retMsg->setIndex(objMsg->getIndex());
                retMsg->setSubIndex(objMsg->getSubIndex());
                retMsg->setSdoType(objMsg->getSdoType());
                retMsg->setUserArg(objMsg->getUserArg());

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::finishUserObdAccess: {
            // cast message
            auto connMsg =
                    dynamic_cast<oplkMessages::ObdAlConnectionMessage*>(msg);

            if ((connMsg != nullptr) && (mApi.finishUserObdAccess != nullptr))
            {
                UINT size = connMsg->getDataArraySize();
                std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });

                // copy transmitted data
                for (auto i = 0u; i < size; i++)
                    data[i] = connMsg->getData(i);

                auto connHdl = connMsg->getObdAlConnHdl();
                connHdl.pDstData = data.get();
                connHdl.pSrcData = data.get();

                ret = mApi.finishUserObdAccess(&connHdl);

                // create return message
                auto retMsg = new oplkMessages::ObdAlConnectionReturnMessage(
                        "ObdAlConnectionReturn", msg->getKind());
                retMsg->setDataArraySize(size);
                // copy data
                for (auto i = 0u; i < size; i++)
                    connMsg->setData(i, data[i]);

                retMsg->setObdAlConnHdl(connHdl);

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::enableUserObdAccess: {
            // cast message
            auto enableMsg = dynamic_cast<oplkMessages::BoolMessage*>(msg);

            if ((enableMsg != nullptr) && (mApi.enableUserObdAccess != nullptr))
            {
                ret = mApi.enableUserObdAccess(enableMsg->getValue());
            }
            break;
        }
        case ApiCallType::freeSdoChannel: {
            // cast message
            auto sdoMsg = dynamic_cast<oplkMessages::SdoMessage*>(msg);

            if ((sdoMsg != nullptr) && (mApi.freeSdoChannel != nullptr))
            {
                ret = mApi.freeSdoChannel(sdoMsg->getComConHdl());
            }
            break;
        }
        case ApiCallType::abortSdoChannel: {
            // cast message
            auto sdoMsg = dynamic_cast<oplkMessages::SdoMessage*>(msg);

            if ((sdoMsg != nullptr) && (mApi.abortSdoChannel != nullptr))
            {
                ret = mApi.abortSdoChannel(sdoMsg->getComConHdl(),
                        sdoMsg->getAbortCode());
            }
            break;
        }
        case ApiCallType::readLocalObject: {
            // cast message
            auto objMsg = dynamic_cast<oplkMessages::LocalObjectMessage*>(msg);

            if ((objMsg != nullptr) && (mApi.readLocalObject != nullptr))
            {
                UINT size = objMsg->getDataArraySize();
                std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });

                ret = mApi.readLocalObject(objMsg->getIndex(),
                        objMsg->getSubIndex(), data.get(), &size);

                // create return message
                auto retMsg = new oplkMessages::LocalObjectReturnMessage(
                        "ReadLocalObjectReturn", msg->getKind());

                retMsg->setDataArraySize(size);
                // copy data
                for (auto i = 0u; i < size; i++)
                    retMsg->setData(i, data[i]);
                retMsg->setIndex(objMsg->getIndex());
                retMsg->setSubIndex(objMsg->getSubIndex());

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::writeLocalObject: {
            // cast message
            auto objMsg = dynamic_cast<oplkMessages::LocalObjectMessage*>(msg);

            if ((objMsg != nullptr) && (mApi.writeLocalObject != nullptr))
            {
                UINT size = objMsg->getDataArraySize();
                std::unique_ptr<BYTE[]> data(new BYTE[size] { 0 });
                // copy data
                for (auto i = 0u; i < size; i++)
                    data[i] = objMsg->getData(i);

                ret = mApi.writeLocalObject(objMsg->getIndex(),
                        objMsg->getSubIndex(), data.get(), size);

                // create return message
                auto retMsg = new oplkMessages::LocalObjectReturnMessage(
                        "WriteLocalObjectReturn", msg->getKind());

                retMsg->setDataArraySize(size);
                // copy data
                for (auto i = 0u; i < size; i++)
                    retMsg->setData(i, data[i]);
                retMsg->setIndex(objMsg->getIndex());
                retMsg->setSubIndex(objMsg->getSubIndex());

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::sendAsndFrame: {
            // cast message
            auto framePkt =
                    dynamic_cast<oplkMessages::SendAsndFramePacket*>(msg);

            if ((framePkt != nullptr) && (mApi.sendAsndFrame != nullptr))
            {
                auto frame =
                        dynamic_cast<oplkMessages::AsndFramePacket*>(framePkt->getEncapsulatedPacket());

                if (frame != nullptr)
                {
                    interface::api::AsndFrame frameObj;
                    frameObj.serviceId = frame->getServiceId();
                    // copy payload
                    for (auto i = 0u; i < frame->getPayLoadArraySize(); i++)
                        frameObj.payload.aPayload[i] = frame->getPayLoad(i);

                    ret = mApi.sendAsndFrame(framePkt->getDestNodeId(),
                            &frameObj, framePkt->getAsndSize());
                }
            }
            break;
        }
        case ApiCallType::sendEthFrame: {
            // cast message
            auto plkPkt = dynamic_cast<oplkMessages::SendPlkPacket*>(msg);

            if ((plkPkt != nullptr) && (mApi.sendEthFrame != nullptr))
            {
                auto frame =
                        dynamic_cast<oplkMessages::PlkPacket*>(plkPkt->getEncapsulatedPacket());

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

                    auto ptr = static_cast<BYTE*>(&frameObj.data.asnd.serviceId);

                    // copy payload
                    for (auto i = 0u; i < frame->getPayLoadArraySize(); i++)
                        ptr[i] = frame->getPayLoad(i);

                    ret = mApi.sendEthFrame(&frameObj, plkPkt->getFrameSize());
                }
            }
            break;
        }
        case ApiCallType::setAsndForward: {
            // cast message
            auto filterMsg = dynamic_cast<oplkMessages::AsndFilterMessage*>(msg);

            if ((filterMsg != nullptr) && (mApi.setAsndForward != nullptr))
            {
                ret = mApi.setAsndForward(filterMsg->getServiceId(),
                        filterMsg->getFilterType());
            }
            break;
        }
        case ApiCallType::setNonPlkForward: {
            // cast message
            auto fwdMsg = dynamic_cast<oplkMessages::BoolMessage*>(msg);

            if ((fwdMsg != nullptr) && (mApi.setNonPlkForward != nullptr))
            {
                ret = mApi.setNonPlkForward(fwdMsg->getValue());
            }
            break;
        }
        case ApiCallType::postUserEvent: {
            // cast message
            auto eventMsg = dynamic_cast<oplkMessages::PointerContMessage*>(msg);

            if ((eventMsg != nullptr) && (mApi.postUserEvent != nullptr))
            {
                ret = mApi.postUserEvent((void*) eventMsg->getPointer());
            }
            break;
        }
        case ApiCallType::triggerMnStateChange: {
            // cast message
            auto nmtMsg = dynamic_cast<oplkMessages::NmtNodeMessage*>(msg);

            if ((nmtMsg != nullptr) && (mApi.triggerMnStateChange != nullptr))
            {
                ret = mApi.triggerMnStateChange(nmtMsg->getNodeId(),
                        nmtMsg->getNodeCommand());
            }
            break;
        }
        case ApiCallType::setCdcBuffer: {
            // cast message
            auto bufferMsg = dynamic_cast<oplkMessages::BufferMessage*>(msg);

            if ((bufferMsg != nullptr) && (mApi.setCdcBuffer != nullptr))
            {
                // allocate new buffer with transmitted data
                auto buffer = new BYTE[bufferMsg->getBufferArraySize()];
                for (auto i = 0u; i < bufferMsg->getBufferArraySize(); i++)
                    buffer[i] = bufferMsg->getBuffer(i);

                // store buffer in internal container
                mStoredBuffer.emplace_back(buffer);

                ret = mApi.setCdcBuffer(buffer,
                        bufferMsg->getBufferArraySize());
            }
        }
            break;
        case ApiCallType::setOdArchivePath: {
            // cast message
            auto archiveMsg = dynamic_cast<oplkMessages::StringMessage*>(msg);

            if ((archiveMsg != nullptr) && (mApi.setOdArchivePath != nullptr))
            {
                ret = mApi.setOdArchivePath(archiveMsg->getString());
            }
            break;
        }
        case ApiCallType::setCdcFilename: {
            // cast message
            auto cdcMsg = dynamic_cast<oplkMessages::PointerContMessage*>(msg);

            if ((cdcMsg != nullptr) && (mApi.setCdcFilename != nullptr))
            {
                const char* cdcFile = (const char*) cdcMsg->getPointer();

                ret = mApi.setCdcFilename(cdcFile);
            }
            break;
        }
        case ApiCallType::process:
            ret = mApi.process();
            break;
        case ApiCallType::getIdentResponse: {
            // cast message
            auto nodeMsg = dynamic_cast<oplkMessages::UintMessage*>(msg);

            if ((nodeMsg != nullptr) && (mApi.getIdentResponse != nullptr))
            {
                interface::api::IdentResponse* identResponse;

                ret = mApi.getIdentResponse(nodeMsg->getValue(),
                        &identResponse);

                // create returning packet with encapsulated identresponse
                auto identPkt = new oplkMessages::IdentResponsePacket();
                identPkt->setFlag1(identResponse->flag1);
                identPkt->setFlag2(identResponse->flag2);
                identPkt->setNmtStatus(identResponse->nmtStatus);
                identPkt->setIdentResponseFlags(
                        identResponse->identResponseFlags);
                identPkt->setPowerlinkProfileVersion(
                        identResponse->powerlinkProfileVersion);
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
                identPkt->setVerifyConfigurationDateLe(
                        identResponse->verifyConfigurationDateLe);
                identPkt->setVerifyConfigurationTimeLe(
                        identResponse->verifyConfigurationTimeLe);
                identPkt->setApplicationSwDateLe(
                        identResponse->applicationSwDateLe);
                identPkt->setApplicationSwTimeLe(
                        identResponse->applicationSwTimeLe);
                identPkt->setIpAddressLe(identResponse->ipAddressLe);
                identPkt->setSubnetMaskLe(identResponse->subnetMaskLe);
                identPkt->setDefaultGatewayLe(identResponse->defaultGatewayLe);

                // Hostname
                for (auto i = 0u; i < sizeof(identResponse->sHostName); i++)
                    identPkt->setHostName(i, identResponse->sHostName[i]);

                // vendor specific ext 2
                for (auto i = 0u;
                        i < sizeof(identResponse->aVendorSpecificExt2); i++)
                    identPkt->setVendorSpecificExt2(i,
                            identResponse->aVendorSpecificExt2[i]);

                auto retPkt = new oplkMessages::ReturnPacket(
                        "GetIdentResponseReturn", msg->getKind());
                retPkt->encapsulate(identPkt);

                retPtr = retPkt;
            }
            break;
        }
        case ApiCallType::getEthMacAddr: {
            if (mApi.getEthMacAddr != nullptr)
            {
                BYTE mac[6];
                ret = mApi.getEthMacAddr(mac);

                auto macMsg = new oplkMessages::MacReturnMessage("GetMacReturn",
                        msg->getKind());

                for (auto i = 0u; i < sizeof(mac); i++)
                    macMsg->setMac(i, mac[i]);

                retPtr = macMsg;
            }
            break;
        }
        case ApiCallType::checkKernelStack: {
            if (mApi.checkKernelStack != nullptr)
            {
                auto valid = mApi.checkKernelStack();

                auto checkMsg = new oplkMessages::BoolMessage(
                        "CheckKernelStackReturn", msg->getKind());
                checkMsg->setValue(valid);

                retPtr = checkMsg;
            }
            break;
        }
        case ApiCallType::waitSyncEvent: {
            // cast message
            auto timeoutMsg = dynamic_cast<oplkMessages::UlongMessage*>(msg);

            if ((timeoutMsg != nullptr) && (mApi.waitSyncEvent != nullptr))
            {
                ret = mApi.waitSyncEvent(timeoutMsg->getValue());
            }
            break;
        }
        case ApiCallType::getVersion: {
            if (mApi.getVersion != nullptr)
            {
                auto version = mApi.getVersion();

                auto versionMsg = new oplkMessages::UintMessage(
                        "GetVersionReturn", msg->getKind());

                versionMsg->setValue(version);

                retPtr = versionMsg;
            }
            break;
        }
        case ApiCallType::getVersionString: {
            if (mApi.getVersionString != nullptr)
            {
                auto str = mApi.getVersionString();

                auto versionMsg = new oplkMessages::StringMessage(
                        "GetVersionStringReturn", msg->getKind());

                versionMsg->setString(str);

                retPtr = versionMsg;
            }
            break;
        }
        case ApiCallType::getStackConfiguration: {
            if (mApi.getStackConfiguration != nullptr)
            {
                auto conf = mApi.getStackConfiguration();

                auto confMsg = new oplkMessages::UintMessage(
                        "GetStackConfigurationReturn", msg->getKind());

                confMsg->setValue(conf);

                retPtr = confMsg;
            }
            break;
        }
        case ApiCallType::getStackInfo: {
            if (mApi.getStackInfo != nullptr)
            {
                interface::api::ApiStackInfo info;

                ret = mApi.getStackInfo(&info);

                auto infoMsg = new oplkMessages::StackInfoReturnMessage(
                        "GetStackInfoReturn", msg->getKind());

                infoMsg->setInfo(info);

                retPtr = infoMsg;
            }
            break;
        }
        case ApiCallType::getSocTime: {
            if (mApi.getSocTime != nullptr)
            {
                interface::api::SocTimeInfo info;

                ret = mApi.getSocTime(&info);

                auto infoMsg = new oplkMessages::SocTimeReturnMessage(
                        "GetSocTimeReturn", msg->getKind());

                infoMsg->setInfo(info);

                retPtr = infoMsg;
            }
            break;
        }

        case ApiCallType::allocProcessImage: {
            // cast message
            auto sizeMsg =
                    dynamic_cast<oplkMessages::ProcessImageSIzeMessage*>(msg);

            if ((sizeMsg != nullptr) && (mApi.allocProcessImage != nullptr))
            {
                ret = mApi.allocProcessImage(sizeMsg->getSizeIn(),
                        sizeMsg->getSizeOut());
            }
            break;
        }
        case ApiCallType::freeProcessImage:
            if (mApi.freeProcessImage != nullptr)
                ret = mApi.freeProcessImage();
            break;
        case ApiCallType::linkProcessImageObject: {
            // cast message
            auto linkMsg =
                    dynamic_cast<oplkMessages::LinkProcessImageMessage*>(msg);

            if ((linkMsg != nullptr)
                    && (mApi.linkProcessImageObject != nullptr))
            {
                UINT varEntries = linkMsg->getVarEntries();
                ret = mApi.linkProcessImageObject(linkMsg->getObjIndex(),
                        linkMsg->getFirstSubIndex(), linkMsg->getOffset(),
                        linkMsg->getOutputPi(), linkMsg->getEntrySize(),
                        &varEntries);

                // create return message
                auto retMsg = new oplkMessages::LinkProcessImageReturnMessage(
                        "LinkProcessImageReturn", msg->getKind());
                retMsg->setVarEntries(varEntries);

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::exchangeProcessImageIn:
            if (mApi.exchangeProcessImageIn != nullptr)
                ret = mApi.exchangeProcessImageIn();
            break;
        case ApiCallType::exchangeProcessImageOut:
            if (mApi.exchangeProcessImageOut != nullptr)
                ret = mApi.exchangeProcessImageOut();
            break;
        case ApiCallType::getProcessImageIn: {
            if (mApi.getProcessImageIn != nullptr)
            {
                auto img = mApi.getProcessImageIn();

                auto retMsg = new oplkMessages::PointerContMessage(
                        "GetProcessImageInReturn", msg->getKind());
                retMsg->setPointer((oplkMessages::PointerCont) img);

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::getProcessImageOut: {
            if (mApi.getProcessImageOut != nullptr)
            {
                auto img = mApi.getProcessImageOut();

                auto retMsg = new oplkMessages::PointerContMessage(
                        "GetProcessImageOutReturn", msg->getKind());
                retMsg->setPointer((oplkMessages::PointerCont) img);

                retPtr = retMsg;
            }
            break;
        }
        case ApiCallType::setupProcessImage:
            if (mApi.setupProcessImage != nullptr)
                ret = mApi.setupProcessImage();
            break;

        case ApiCallType::triggerPresForward: {
            // cast message
            auto nodeMsg = dynamic_cast<oplkMessages::UintMessage*>(msg);

            if ((nodeMsg != nullptr) && (mApi.triggerPresForward != nullptr))
            {
                ret = mApi.triggerPresForward(nodeMsg->getValue());
            }
            break;
        }
        default:
            error("%s - unknown message kind received %d", __PRETTY_FUNCTION__,
                    msg->getKind());
    }

    EV << "Executed " << getApiCallString(callType) << std::endl;

    // check if alternative return message was not set
    if (retPtr == nullptr)
    {
        // create return message
        auto retMsg = new oplkMessages::ReturnMessage();
        retPtr = retMsg;
    }

    string recvName(msg->getName());

    // set name of return message
    retPtr->setName(("Return - " + recvName).c_str());

    auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(retPtr);
    if (retMsg != nullptr)
        retMsg->setReturnValue(ret);
    else
    {
        auto retPkt = dynamic_cast<oplkMessages::ReturnPacket*>(retPtr);
        if (retPkt != nullptr)
            retPkt->setReturnValue(ret);
    }

    // set received message kind and send message
    retPtr->setKind(msg->getKind());
    sendReturnMessage(retPtr, msg->getArrivalGate()->getIndex());
}

void Api::handleAppReturn(RawMessagePtr msg)
{
    if (msg != nullptr)
    {
        // check type
        switch (static_cast<AppBase::AppBaseCallType>(msg->getKind()))
        {
            case AppBase::AppBaseCallType::processSync: {
                // cast message
                auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg);

                if (retMsg != nullptr)
                {
                    // set recceiving info
                    mProcessSyncInfo.returnValue = retMsg->getReturnValue();
                    mProcessSyncInfo.received = true;
                }

                break;
            }

            default:
                error("unexpected or invalid app return type: %d",
                        msg->getKind());
        }
    }
}

void Api::sendReturnMessage(cMessage* msg, size_t gateIdx)
{
    send(msg, mReturnGates.at(gateIdx));
}

interface::api::ApiFunctions* Api::getApiFunctions()
{
    return &mApi;
}

const char* Api::getApiCallString(ApiCallType type)
{
    return cApiCallNames[static_cast<size_t>(type)];
}
