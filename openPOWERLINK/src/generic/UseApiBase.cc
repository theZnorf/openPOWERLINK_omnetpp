/*
 * UseApiBase.cc
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#include <string>
#include <UseApiBase.h>
#include "oplkMessages/ApiMessages.h"
#include "OplkException.h"

USING_NAMESPACE

UseApiBase::UseApiBase(std::string const & sendGateName) :
        SendAwaitedReturnBase(sendGateName, [](UseApiBase::RawMessagePtr msg, UseApiBase::Kind kind)
        {   msg->setKind(static_cast<short>(kind));}, [](UseApiBase::RawMessagePtr msg)
        {   return static_cast<UseApiBase::Kind>(msg->getKind());})
{
}

UseApiBase::MessagePtr UseApiBase::sendMessageWithCallTypeAndWaitForReturn(RawMessagePtr msg, CallType calltype)
{
    // send awaited message with call type
    sendAwaitedMessage(msg, calltype);

    // receive and return awaited message
    return waitForReturnMessage(calltype);
}

void UseApiBase::checkReturnMessage(MessagePtr msg, std::string const & errorMessage)
{
    auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg.get());

    if (retMsg != nullptr)
    {
        if (retMsg->getReturnValue() != Error::kErrorOk)
        {
            auto except = interface::OplkException(errorMessage, retMsg->getReturnValue());

            EV << except.what() << std::endl;

            throw except;
        }
    }
}

void UseApiBase::initStack()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::init);

    checkReturnMessage(ret, "Initialize stack");
}

void UseApiBase::createStack(interface::api::ApiInitParam& param)
{
    // create message
    auto msg = new oplkMessages::InitMessage();
    msg->setName(__FUNCTION__);
    msg->setInitParam(param);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::create);

    checkReturnMessage(ret, "create stack");
}

void UseApiBase::setupProcessImage()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::setupProcessImage);

    checkReturnMessage(ret, "setup process image");
}

void UseApiBase::destroyStack()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::destroy);

    checkReturnMessage(ret, "Destroy stack");
}

void UseApiBase::exitStack()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::exit);

    checkReturnMessage(ret, "Exit stack");
}

void UseApiBase::execNmtCommand(interface::api::NmtEvent event)
{
    // create message
    auto msg = new oplkMessages::NmtMessage();
    msg->setName(__FUNCTION__);
    msg->setNmtEvent(event);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::execNmtCommand);

    checkReturnMessage(ret, "Execute nmt command");
}

void UseApiBase::cbGenericObdAccess(interface::api::ObdCallbackParam* param)
{
    // create message
    auto msg = new oplkMessages::ObdCbMessage();
    msg->setName(__FUNCTION__);
    msg->setObdCbParam(*param);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::cbGenericObdAccess);

    checkReturnMessage(ret, "cb generic obd access");
}

void UseApiBase::linkObject(unsigned int objIndex, void* var, unsigned int* varEntries,
        interface::api::ObdSize* entrySize, unsigned int firstSubIndex)
{
    // create message
    auto msg = new oplkMessages::LinkMessage();
    msg->setName(__FUNCTION__);
    msg->setObjIndex(objIndex);
    msg->setVariable((oplkMessages::PointerCont) var);
    msg->setVarEntries(*varEntries);
    msg->setEntrySize(*entrySize);
    msg->setFirstSubIndex(firstSubIndex);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::linkObject);

    checkReturnMessage(ret, "link object");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::LinkReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        *varEntries = retMsg->getVarEntries();
        *entrySize = retMsg->getEntrySize();
    }
}

void UseApiBase::readObject(interface::api::SdoComConHdl* hdl, unsigned int nodeId, unsigned int index,
        unsigned int subIndex, void* dstDataLe, unsigned int* size, interface::api::SdoType sdoType, void* userArg)
{
    // create message
    auto msg = new oplkMessages::ObjectMessage();
    msg->setName(__FUNCTION__);
    msg->setSdoComConHdl(*hdl);
    msg->setNodeId(nodeId);
    msg->setIndex(index);
    msg->setSubIndex(subIndex);
    msg->setObjDataArraySize(*size);
    msg->setSdoType(sdoType);
    msg->setUserArg((oplkMessages::PointerCont) userArg);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::readObject);

    checkReturnMessage(ret, "read object");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::ObjectReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        *hdl = retMsg->getSdoComConHdl();
        *size = retMsg->getObjDataArraySize();
        for (auto i = 0u; i < *size; i++)
            static_cast<BYTE*>(dstDataLe)[i] = retMsg->getObjData(i);
    }
}

void UseApiBase::writeObject(interface::api::SdoComConHdl* hdl, unsigned int nodeId, unsigned int index,
        unsigned int subIndex, void* srcDataLe, unsigned int size, interface::api::SdoType sdoType, void* userArg)
{
    // create message
    auto msg = new oplkMessages::ObjectMessage();
    msg->setName(__FUNCTION__);
    msg->setSdoComConHdl(*hdl);
    msg->setNodeId(nodeId);
    msg->setIndex(index);
    msg->setSubIndex(subIndex);
    msg->setObjDataArraySize(size);
    for (auto i = 0u; i < size; i++)
        msg->setObjData(i, static_cast<BYTE*>(srcDataLe)[i]);
    msg->setSdoType(sdoType);
    msg->setUserArg((oplkMessages::PointerCont) userArg);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::writeObject);

    checkReturnMessage(ret, "write object");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::ObjectReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        *hdl = retMsg->getSdoComConHdl();
    }
}

void UseApiBase::finishUserAccess(interface::api::ObdAlConnHdl* userObdConHdl)
{
    // create message
    auto msg = new oplkMessages::ObdAlConnectionMessage();
    msg->setName(__FUNCTION__);
    msg->setObdAlConnHdl(*userObdConHdl);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::finishUserObdAccess);

    checkReturnMessage(ret, "finish user access");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::ObdAlConnectionReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        *userObdConHdl = retMsg->getObdAlConnHdl();
    }
}

void UseApiBase::enableUserAccess(bool enable)
{
    // create message
    auto msg = new oplkMessages::BoolMessage();
    msg->setName(__FUNCTION__);
    msg->setValue(enable);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::enableUserObdAccess);

    checkReturnMessage(ret, "enable user access");
}

void UseApiBase::freeSdoChannel(interface::api::SdoComConHdl sdoComConHdl)
{
    // create message
    auto msg = new oplkMessages::SdoMessage();
    msg->setName(__FUNCTION__);
    msg->setComConHdl(sdoComConHdl);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::freeSdoChannel);

    checkReturnMessage(ret, "free sdo channel");
}

void UseApiBase::abortSdo(interface::api::SdoComConHdl sdoComConHdl, unsigned int abortCode)
{
    // create message
    auto msg = new oplkMessages::SdoMessage();
    msg->setName(__FUNCTION__);
    msg->setComConHdl(sdoComConHdl);
    msg->setAbortCode(abortCode);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::freeSdoChannel);

    checkReturnMessage(ret, "abort sdo");
}

void UseApiBase::readLocalObject(unsigned int index, unsigned int subIndex, void* dstData, unsigned int* size)
{
    // create message
    auto msg = new oplkMessages::LocalObjectMessage();
    msg->setName(__FUNCTION__);
    msg->setIndex(index);
    msg->setSubIndex(subIndex);
    msg->setDataArraySize(*size);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::readLocalObject);

    checkReturnMessage(ret, "read local object");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::LocalObjectReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        *size = retMsg->getDataArraySize();
        for (auto i = 0u; i < *size; i++)
            static_cast<BYTE*>(dstData)[i] = retMsg->getData(i);
    }
}

void UseApiBase::writeLocalObject(unsigned int index, unsigned int subIndex, void* srcData, unsigned int size)
{
    // create message
    auto msg = new oplkMessages::LocalObjectMessage();
    msg->setName(__FUNCTION__);
    msg->setIndex(index);
    msg->setSubIndex(subIndex);
    msg->setDataArraySize(size);
    for (auto i = 0u; i < size; i++)
        msg->setData(i, static_cast<BYTE*>(srcData)[i]);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::writeLocalObject);

    checkReturnMessage(ret, "write local object");
}

void UseApiBase::sendAsndFrame(unsigned char dstNodeId, interface::api::AsndFrame* asndFrame, size_t asndSize)
{
    // create message
    auto pkt = new oplkMessages::SendAsndFramePacket();
    pkt->setName(__FUNCTION__);
    pkt->setDestNodeId(dstNodeId);
    pkt->setAsndSize(asndSize);

    // encapluslate asnd frame packet
    auto asndPkt = new oplkMessages::AsndFramePacket();
    asndPkt->setServiceId(asndFrame->serviceId);
    // copy payload
    for (auto i = 0u; i < asndPkt->getPayLoadArraySize(); i++)
        asndPkt->setPayLoad(i, asndFrame->payload.aPayload[i]);

    pkt->encapsulate(asndPkt);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(pkt, CallType::sendAsndFrame);

    checkReturnMessage(ret, "send asnd frame");
}

void UseApiBase::sendEthFrame(interface::api::PlkFrame* frame, unsigned int frameSize)
{
    // create message
    auto pkt = new oplkMessages::SendPlkPacket();
    pkt->setName(__FUNCTION__);
    pkt->setFrameSize(frameSize);

    // encapluslate plk frame packet
    auto plkPkt = new oplkMessages::PlkPacket();
    plkPkt->setEtherType(frame->etherType);
    plkPkt->setMsgType(frame->messageType);
    plkPkt->setDstNodeId(frame->dstNodeId);
    plkPkt->setSrcNodeId(frame->srcNodeId);
    // copy mac addresses
    for (auto i = 0u; i < plkPkt->getDstMacArraySize(); i++)
    {
        plkPkt->setDstMac(i, frame->aDstMac[i]);
        plkPkt->setSrcMac(i, frame->aDstMac[i]);
    }
    // copy payload
    for (auto i = 0u; i < plkPkt->getPayLoadArraySize(); i++)
        plkPkt->setPayLoad(i, static_cast<BYTE*>(&frame->data.asnd.serviceId)[i]);

    pkt->encapsulate(plkPkt);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(pkt, CallType::sendEthFrame);

    checkReturnMessage(ret, "send eth frame");
}

void UseApiBase::setAsndForward(unsigned char serviceId, interface::api::AsndFilter filterType)
{
    // create message
    auto msg = new oplkMessages::AsndFilterMessage();
    msg->setName(__FUNCTION__);
    msg->setServiceId(serviceId);
    msg->setFilterType(filterType);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::setAsndForward);

    checkReturnMessage(ret, "set asnd forward");
}

void UseApiBase::setNonPlkForward(bool enable)
{
    // create message
    auto msg = new oplkMessages::BoolMessage();
    msg->setName(__FUNCTION__);
    msg->setValue(enable);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::setNonPlkForward);

    checkReturnMessage(ret, "set non plk forward");
}

void UseApiBase::postUserEvent(void* userArg)
{
    // create message
    auto msg = new oplkMessages::PointerContMessage();
    msg->setName(__FUNCTION__);
    msg->setPointer((oplkMessages::PointerCont) userArg);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::postUserEvent);

    checkReturnMessage(ret, "post user event");
}

void UseApiBase::triggerMnStateChange(unsigned int nodeId, interface::api::NmtNodeCommand nodeCommand)
{
    // create message
    auto msg = new oplkMessages::NmtNodeMessage();
    msg->setName(__FUNCTION__);
    msg->setNodeId(nodeId);
    msg->setNodeCommand(nodeCommand);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::triggerMnStateChange);

    checkReturnMessage(ret, "trigger nmt state change");
}

void UseApiBase::setCdcBuffer(unsigned char* cdc, unsigned int cdcSize)
{
    // create message
    auto msg = new oplkMessages::BufferMessage();
    msg->setName(__FUNCTION__);
    msg->setBufferArraySize(cdcSize);
    for (auto i = 0u; i < cdcSize; i++)
        msg->setBuffer(i, cdc[i]);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::setCdcBuffer);

    checkReturnMessage(ret, "set cdc buffer");
}

void UseApiBase::setCdcFilename(const char * fileName)
{
    // create message
    auto msg = new oplkMessages::PointerContMessage();
    msg->setName(__FUNCTION__);
    msg->setPointer((oplkMessages::PointerCont) fileName);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::setCdcFilename);

    checkReturnMessage(ret, "set cdc filename");
}

void UseApiBase::setOdArchivePath(const std::string& backUpPath)
{
    // create message
    auto msg = new oplkMessages::StringMessage();
    msg->setName(__FUNCTION__);
    msg->setString(backUpPath.c_str());

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::setOdArchivePath);

    checkReturnMessage(ret, "set od archive path");
}

void UseApiBase::stackProcess()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::process);

    checkReturnMessage(ret, "stack process");
}

void UseApiBase::getIdentResponse(unsigned int nodeId, interface::api::IdentResponse** identResponse)
{
    // create message
    auto msg = new oplkMessages::UintMessage();
    msg->setName(__FUNCTION__);
    msg->setValue(nodeId);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getIdentResponse);

    checkReturnMessage(ret, "get ident response");

    // cast packet
    auto retPkt = dynamic_cast<oplkMessages::ReturnPacket*>(ret.get());

    if (retPkt != nullptr)
    {
        //TODO: implement and solve memory issue
    }
}

void UseApiBase::getEthMacAddr(unsigned char* macAddr)
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getEthMacAddr);

    checkReturnMessage(ret, "get mac addr");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::MacReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        for (auto i = 0u; i < retMsg->getMacArraySize(); i++)
            macAddr[i] = retMsg->getMac(i);
    }
}

bool UseApiBase::checkKernelStack()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::checkKernelStack);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::BoolMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        return retMsg->getValue();
    }

    return false;
}

void UseApiBase::waitSyncEvent(unsigned long int timeout)
{
    // create message
    auto msg = new oplkMessages::UlongMessage();
    msg->setName(__FUNCTION__);
    msg->setValue(timeout);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::waitSyncEvent);

    checkReturnMessage(ret, "wait sync event");
}

unsigned int UseApiBase::getVersion()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getVersion);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::UintMessage*>(ret.get());
    if (retMsg != nullptr)
        return retMsg->getValue();
    return 0;
}

std::string UseApiBase::getVersionString()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getVersionString);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::StringMessage*>(ret.get());
    if (retMsg != nullptr)
        return retMsg->getString();
    return "";
}

unsigned int UseApiBase::getStackConfiguration()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getStackConfiguration);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::UintMessage*>(ret.get());
    if (retMsg != nullptr)
        return retMsg->getValue();
    return 0;
}

void UseApiBase::getStackInfo(interface::api::ApiStackInfo* stackInfo)
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getStackConfiguration);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::StackInfoReturnMessage*>(ret.get());
    if (retMsg != nullptr)
        *stackInfo = retMsg->getInfo();
}

void UseApiBase::getSocTime(interface::api::SocTimeInfo* timeInfo)
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getSocTime);

    checkReturnMessage(ret, "get soc time");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::SocTimeReturnMessage*>(ret.get());
    if (retMsg != nullptr)
        *timeInfo = retMsg->getInfo();
}

void UseApiBase::allocProcessImage(unsigned int sizeProcessImageIn, unsigned int sizeProcessImageOut)
{
    // create message
    auto msg = new oplkMessages::ProcessImageSIzeMessage();
    msg->setName(__FUNCTION__);
    msg->setSizeIn(sizeProcessImageIn);
    msg->setSizeOut(sizeProcessImageOut);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::allocProcessImage);

    checkReturnMessage(ret, "allocate process image");
}

void UseApiBase::freeProcessImage()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::freeProcessImage);

    checkReturnMessage(ret, "free process image");
}

void UseApiBase::linkProcessImageObject(unsigned int objIndex, unsigned int firstSubIndex, unsigned int offsetPI,
        bool outputPI, interface::api::ObdSize entrySize, unsigned int* varEntries)
{
    // create message
    auto msg = new oplkMessages::LinkProcessImageMessage();
    msg->setName(__FUNCTION__);
    msg->setObjIndex(objIndex);
    msg->setFirstSubIndex(firstSubIndex);
    msg->setOffset(offsetPI);
    msg->setOutputPi(outputPI);
    msg->setEntrySize(entrySize);
    msg->setVarEntries(*varEntries);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::linkProcessImageObject);

    checkReturnMessage(ret, "link process image");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::LinkProcessImageReturnMessage*>(ret.get());
    if (retMsg != nullptr)
    {
        *varEntries = retMsg->getVarEntries();
    }
}

void UseApiBase::exchangeProcessImageIn()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::exchangeProcessImageIn);

    checkReturnMessage(ret, "exchange process image in");
}

void UseApiBase::exchangeProcessImageOut()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::exchangeProcessImageOut);

    checkReturnMessage(ret, "exchange process image out");
}

void* UseApiBase::getProcessImageIn()
{
    // create message
    auto msg = new cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getProcessImageIn);

    checkReturnMessage(ret, "get process image in");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::PointerContMessage*>(ret.get());

    if (retMsg != nullptr)
        return (void*) retMsg->getPointer();
    else
        return nullptr;
}

void* UseApiBase::getProcessImageOut()
{
    // create message
    auto msg = new cMessage();
    msg->setName(__FUNCTION__);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::getProcessImageOut);

    checkReturnMessage(ret, "get process image out");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::PointerContMessage*>(ret.get());

    if (retMsg != nullptr)
        return (void*) retMsg->getPointer();
    else
        return nullptr;
}

void UseApiBase::triggerPresForward(UINT nodeId)
{
    // create message
    auto msg = new oplkMessages::UintMessage();
    msg->setName(__FUNCTION__);
    msg->setValue(nodeId);

    auto ret = sendMessageWithCallTypeAndWaitForReturn(msg, CallType::triggerPresForward);

    checkReturnMessage(ret, "trigger pres forward");
}
