/*
 * UseApiBase.cc
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#include <UseApiBase.h>
#include "oplkMessages/ApiMessages.h"
#include "OplkException.h"

USING_NAMESPACE

UseApiBase::UseApiBase(std::string const & sendGateName)
    : cSimpleModule(1), mSendGateName(sendGateName)
{
}

UseApiBase::~UseApiBase()
{
}

void UseApiBase::initialize()
{
    // resolve gate
    mSendGate = gate(mSendGateName.c_str());
}

void UseApiBase::activity()
{
    bool running = true;

    while(running)
    {
        // receive message
        receiveMessage();
    }
}

void UseApiBase::receiveMessage()
{
    // receive message
    MessagePtr msg(receive());

    if (msg != nullptr)
    {
        // check kind
        auto kind = static_cast<CallType>(msg->getKind());

        // check if kind is awaited
        if (mReturnValues.find(kind) != mReturnValues.end())
        {
            mReturnValues[kind] = msg;
        }
        else // forward unexpected message
            handleOtherMessage(msg);
    }
}

void UseApiBase::checkReturnMessage(MessagePtr msg, std::string const & errorMessage)
{
    auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg.get());

    if (retMsg != nullptr)
    {
        if (retMsg->getReturnValue() != Error::kErrorOk)
            throw interface::OplkException(errorMessage, retMsg->getReturnValue());
    }
    //TODO: error handling
}


void UseApiBase::initStack()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setKind(static_cast<short>(CallType::init));

    // define awaited return
    mReturnValues[CallType::init] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::init];

    // receive until return value was reiceived
    while(ret == nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::init];
    }

    mReturnValues.erase(CallType::init);

    checkReturnMessage(ret, "Initialize stack");
}

void UseApiBase::createStack(interface::api::ApiInitParam& param)
{
    // create message
    auto msg = new oplkMessages::InitMessage();
    msg->setKind(static_cast<short>(CallType::create));
    msg->setInitParam(param);

    // define awaited return
    mReturnValues[CallType::create] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::create];

    // receive until return value was reiceived
    while(ret != nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::create];
    }

    mReturnValues.erase(CallType::create);

    checkReturnMessage(ret, "create stack");
}

void UseApiBase::setCdcFile(const std::string& fileName)
{
    // create message
    auto msg = new oplkMessages::StringMessage();
    msg->setKind(static_cast<short>(CallType::setCdcFilename));
    msg->setString(fileName.c_str());

    // define awaited return
    mReturnValues[CallType::setCdcFilename] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::setCdcFilename];

    // receive until return value was reiceived
    while(ret != nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::setCdcFilename];
    }

    mReturnValues.erase(CallType::setCdcFilename);

    checkReturnMessage(ret, "set cdc filename");
}

void UseApiBase::allocProcessImage(unsigned int sizeProcessImageIn, unsigned int sizeProcessImageOut)
{
    // create message
    auto msg = new oplkMessages::ProcessImageSIzeMessage();
    msg->setKind(static_cast<short>(CallType::allocProcessImage));
    msg->setSizeIn(sizeProcessImageIn);
    msg->setSizeOut(sizeProcessImageOut);

    // define awaited return
    mReturnValues[CallType::allocProcessImage] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::allocProcessImage];

    // receive until return value was reiceived
    while(ret != nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::allocProcessImage];
    }

    mReturnValues.erase(CallType::allocProcessImage);

    checkReturnMessage(ret, "allocate process image");
}

void* UseApiBase::getProcessImageIn()
{
    // create message
    auto msg = new cMessage();
    msg->setKind(static_cast<short>(CallType::getProcessImageIn));

    // define awaited return
    mReturnValues[CallType::getProcessImageIn] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::getProcessImageIn];

    // receive until return value was reiceived
    while(ret != nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::getProcessImageIn];
    }

    mReturnValues.erase(CallType::getProcessImageIn);

    checkReturnMessage(ret, "get process image in");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::PointerContMessage*>(ret.get());

    if (retMsg != nullptr)
        return (void*)retMsg->getPointer();
    else
        return nullptr;
}

void* UseApiBase::getProcessImageOut()
{
    // create message
    auto msg = new cMessage();
    msg->setKind(static_cast<short>(CallType::getProcessImageOut));

    // define awaited return
    mReturnValues[CallType::getProcessImageOut] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::getProcessImageOut];

    // receive until return value was reiceived
    while(ret != nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::getProcessImageOut];
    }

    mReturnValues.erase(CallType::getProcessImageOut);

    checkReturnMessage(ret, "get process image out");

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::PointerContMessage*>(ret.get());

    if (retMsg != nullptr)
        return (void*)retMsg->getPointer();
    else
        return nullptr;
}

void UseApiBase::setupProcessImage()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setKind(static_cast<short>(CallType::setupProcessImage));

    // define awaited return
    mReturnValues[CallType::setupProcessImage] = nullptr;

    // send
    send(msg, mSendGate);

    auto ret = mReturnValues[CallType::setupProcessImage];

    // receive until return value was reiceived
    while(ret == nullptr)
    {
        receiveMessage();

        ret = mReturnValues[CallType::setupProcessImage];
    }

    mReturnValues.erase(CallType::setupProcessImage);

    checkReturnMessage(ret, "setup process image");
}
