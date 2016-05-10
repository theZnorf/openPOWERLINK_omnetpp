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

void UseApiBase::initStack()
{
    // create message
    auto msg = new OPP::cMessage();
    msg->setKind(static_cast<short>(CallType::init));

    // define awaited return
    mReturnValues[CallType::init] = cInvalidError;

    // send
    send(msg, mSendGate);

    ErrorType ret = mReturnValues[CallType::init];

    // receive until return value was reiceived
    while(ret == cInvalidError)
    {
        receiveMessage();

        ret = mReturnValues[CallType::init];
    }

    mReturnValues.erase(CallType::init);

    checkReturnValue(ret, "Initialize stack");
}

void UseApiBase::createStack(interface::api::ApiInitParam& param)
{
    // create message
    auto msg = new oplkMessages::InitMessage();
    msg->setKind(static_cast<short>(CallType::create));
    msg->setInitParam(param);

    // define awaited return
    mReturnValues[CallType::create] = cInvalidError;

    // send
    send(msg, mSendGate);

    ErrorType ret;

    // receive until return value was reiceived
    while(ret != cInvalidError)
    {
        receiveMessage();

        ret = mReturnValues[CallType::create];
    }

    mReturnValues.erase(CallType::create);

    checkReturnValue(ret, "create stack");
}

void UseApiBase::setCdcFile(const std::string& fileName)
{
    // create message
    auto msg = new oplkMessages::StringMessage();
    msg->setKind(static_cast<short>(CallType::setCdcFilename));
    msg->setString(fileName.c_str());

    // define awaited return
    mReturnValues[CallType::setCdcFilename] = cInvalidError;

    // send
    send(msg, mSendGate);

    ErrorType ret;

    // receive until return value was reiceived
    while(ret != cInvalidError)
    {
        receiveMessage();

        ret = mReturnValues[CallType::setCdcFilename];
    }

    mReturnValues.erase(CallType::setCdcFilename);

    checkReturnValue(ret, "set cdc filename stack");
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
            // save return value
            auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg.get());

            if (retMsg != nullptr)
                mReturnValues[kind] = retMsg->getReturnValue();
            //TODO: error handling
        }
        else // forward unexpected message
            handleOtherMessage(msg);
    }
}

void UseApiBase::checkReturnValue(ErrorType err, std::string const & msg)
{
    if (err != Error::kErrorOk)
        throw interface::OplkException(msg, err);
}
