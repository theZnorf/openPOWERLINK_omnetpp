/**
 ********************************************************************************
 \file   DemoBase.cc

 \brief  Implementation of Base class of Demo modules.

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#include <functional>
#include <string>
#include "DemoBase.h"
#include "MsgPtr.h"
#include "stack/Api.h"
#include "InitMessage_m.h"
#include "StringMessage_m.h"
#include "AppBase.h"
#include "OplkException.h"

using namespace std;
USING_NAMESPACE

DemoBase::DemoBase() :
        UseApiBase("apiCall")
{
}

void DemoBase::initialize()
{
    UseApiBase::initialize();

    // resovle parameters
    mStartUpDelay = simtime_t(par("startUpDelay").doubleValue(), SimTimeUnit::SIMTIME_NS);
    mMainLoopInterval = simtime_t(par("mainLoopInterval").doubleValue(), SimTimeUnit::SIMTIME_NS);
    mShutdownTime = simtime_t(par("shutdownTime").doubleValue(), SimTimeUnit::SIMTIME_NS);

    // resolve gates
    mApiCallGate = gate("apiCall");
    mAppCallGate = gate("appCall");

    // init dispatcher
    mDispatcher.registerFunction(gate("appReturn"), std::bind(&DemoBase::processAppReturn, this, placeholders::_1));
    mDispatcher.registerFunction(gate("stackShutdown"),
            std::bind(&DemoBase::processStackShutdown, this, placeholders::_1));

    // init state
    mState = DemoState::initializePowerlink;

    // schedule init message
    scheduleAt(simTime() + mStartUpDelay, new cMessage(("Init demo - " + string(getParentModule()->getName())).c_str()));
}

void DemoBase::handleOtherMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        // check if external message
        if (!msg->isSelfMessage())
        {
            // process message according to arrival gate via dispatcher
            mDispatcher.dispatch(msg.get());
        }
        else // handle self messages
        {
            handleSelfMessage(msg);
        }
    }
}

void DemoBase::handleSelfMessage(MessagePtr msg)
{
    // check sim time
    if (simTime() > mShutdownTime)
    {
        mState = DemoState::shuttingDown;
    }

    // check current state
    switch (mState)
    {
        case DemoState::initializePowerlink: {
            // init powerlink stack by derived class
            initPowerlink();

            // create and send init message for app module
            send(new cMessage("initialize app", static_cast<short>(AppBase::AppBaseCallType::init)), mAppCallGate);

            // advance to next init state
            mState = DemoState::initializeApp;
            break;
        }
        case DemoState::initializeApp:
            EV << "Unexpected message (" << *msg << ") received within initializeApp state" << endl;
            break;

        case DemoState::swReset: {
            // perform sw reset
            execNmtCommand(interface::api::NmtEventType::kNmtEventSwReset);

            // advance to mainloop state
            mState = DemoState::mainloop;

            // send self message
            scheduleAt(simTime() + mMainLoopInterval, new cMessage("first demo main loop message"));

            EV << "App initialized advancing to main loop state" << endl;
            break;
        }

        case DemoState::mainloop: {
            // process stack
            stackProcess();
            // schedule following main message
            scheduleAt(simTime() + mMainLoopInterval, new cMessage("demo main loop message"));
            break;
        }
        case DemoState::shuttingDown: {
            // shutdown powerlink
            shutdownPowerlink();

            EV << "Stack shutdown succesfully" << endl;
            mRunning = false;
            break;
        }

        case DemoState::shuttingDownApp:
            // shutdown app
            send(new cMessage("shutdown app", static_cast<short>(AppBase::AppBaseCallType::shutdown)), mAppCallGate);
            break;

        default:
            error("DemoBase - invalid demo state: %d", static_cast<short>(mState));
    }
}

void DemoBase::processAppReturn(RawMessagePtr msg)
{
    auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg);
    if (retMsg != nullptr)
    {
        auto ret = retMsg->getReturnValue();

        // check if error
        if (ret != interface::api::Error::kErrorOk)
            throw interface::OplkException("Error in App call ocurred", ret);

        // check if initialization returned
        auto kind = static_cast<AppBase::AppBaseCallType>(retMsg->getKind());
        switch (kind)
        {
            case AppBase::AppBaseCallType::init: {
                // advance to sw reset state
                mState = DemoState::swReset;

                // schedule immediate self message
                scheduleAt(simTime(), new cMessage());
                break;
            }
            case AppBase::AppBaseCallType::shutdown: {
                // advance to shutdown state
                mState = DemoState::shuttingDown;

                // schedule immediate self message
                scheduleAt(simTime(), new cMessage());
                break;
            }
            default:
                error("HandleAppReturn - invalid message kind: %d", static_cast<short>(kind));
        }
    }
}

void DemoBase::processStackShutdown(RawMessagePtr msg)
{
    // advance to shutting down state
    mState = DemoState::shuttingDownApp;
}
