/**
 ********************************************************************************
 \file   AppBase.cc

 \brief  Implementation of Base class of App modules.

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#include <functional>
#include "AppBase.h"

USING_NAMESPACE

AppBase::AppBase() :
        UseApiBase("apiCall")
{
}

void AppBase::initialize()
{
    UseApiBase::initialize();

    for (auto i = 0; i < gateSize("functionReturn"); i++)
    {
        // resolve gates
        mReturnGates.push_back(gate("functionReturn", i));

        // create return handler with send method bound to this and the return gate
        mReturns.emplace_back(new ReturnHandler(std::bind(static_cast<int (AppBase::*)(cMessage*, cGate*)>(&AppBase::send),
                        this, std::placeholders::_1, mReturnGates[i])));
    }
}

void AppBase::handleOtherMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        interface::api::ErrorType ret = interface::api::Error::kErrorOk;

        // check message kind
        switch (static_cast<AppBaseCallType>(msg->getKind()))
        {
            case AppBaseCallType::init:
                ret = this->initApp();
                break;
            case AppBaseCallType::processSync:
                ret = this->processSync();
                break;
            case AppBaseCallType::shutdown:
                this->shutdownApp();
                break;
            default:
                handleAppMessage(msg);
        }

        // send return message
        mReturns.at(msg->getArrivalGate()->getIndex())->sendReturnValue(ret, msg->getKind());
    }
}

void AppBase::handleAppMessage(MessagePtr msg)
{
    // Empty function stub within base class
}
