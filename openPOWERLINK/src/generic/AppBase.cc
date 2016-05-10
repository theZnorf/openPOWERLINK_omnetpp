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

#include <functional>
#include "AppBase.h"
#include "MsgPtr.h"
#include "ApiDef.h"
#include "InitMessage_m.h"
#include "OplkException.h"

#include "xap.h"

USING_NAMESPACE

Define_Module(AppBase);

#define DEFAULT_MAX_CYCLE_COUNT 20      // 6 is very fast
#define APP_LED_COUNT_1         8       // number of LEDs for CN1
#define APP_LED_MASK_1          (1 << (APP_LED_COUNT_1 - 1))
#define MAX_NODES               255

typedef struct
{
        UINT leds;UINT ledsOld;UINT input;UINT inputOld;UINT period;
        int toggle;
} APP_NODE_VAR_T;

static int usedNodeIds_l[] = { 1, 32, 110, 0 };
static UINT cnt_l;
static APP_NODE_VAR_T nodeVar_l[MAX_NODES];
static PI_IN* pProcessImageIn_l;
static PI_OUT* pProcessImageOut_l;

AppBase::AppBase() :
        UseApiBase("apiCall")
{
}

void AppBase::initialize()
{
    UseApiBase::initialize();

    // resolve gate
    mReturnGate = gate("return");

    // create return handler with send method bound to this and the return gate
    mRet = std::make_unique < ReturnHandler
            > (std::bind(static_cast<int (AppBase::*)(cMessage*, cGate*)>(&AppBase::send), this, std::placeholders::_1, mReturnGate));
}

void AppBase::handleOtherMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        auto ret = -1;

        // check message kind
        switch (static_cast<AppCallType>(msg->getKind()))
        {
            case AppCallType::init:
                ret = initApp();
                break;
            case AppCallType::processSync:
                ret = processSync();
                break;
            case AppCallType::shutdown:
                shutdownApp();
                break;
            default:
                error("unknown message kind %d", msg->getKind());
        }

        // check if return value was set
        if (ret != -1)
            mRet->sendReturnValue(ret, msg->getKind());
    }
}

interface::api::ErrorType AppBase::initApp()
{
    interface::api::ErrorType ret = interface::api::Error::kErrorOk;
    try
    {
        int i;

        cnt_l = 0;

        for (i = 0; (i < MAX_NODES) && (usedNodeIds_l[i] != 0); i++)
        {
            nodeVar_l[i].leds = 0;
            nodeVar_l[i].ledsOld = 0;
            nodeVar_l[i].input = 0;
            nodeVar_l[i].inputOld = 0;
            nodeVar_l[i].toggle = 0;
            nodeVar_l[i].period = 0;
        }

        EV << "Initializing process image..." << std::endl;
        EV << "Size of process image: Input = " << (ULONG) sizeof(PI_IN) << " Output = " << (ULONG) sizeof(PI_OUT)
                << std::endl;
        //eventlog_printMessage(kEventlogLevelInfo, kEventlogCategoryGeneric, "Allocating process image: Input:%lu Output:%lu", (ULONG)sizeof(PI_IN), (ULONG)sizeof(PI_OUT));

        allocProcessImage(sizeof(PI_IN), sizeof(PI_OUT));

        pProcessImageIn_l = (PI_IN*) getProcessImageIn();
        pProcessImageOut_l = (PI_OUT*) getProcessImageOut();

        setupProcessImage();

    }
    catch (interface::OplkException const & e)
    {
        ret = e.errorNumber();
    }

    return ret;
}

interface::api::ErrorType AppBase::processSync()
{
    interface::api::ErrorType ret = interface::api::Error::kErrorOk;
    return ret;
}

void AppBase::shutdownApp()
{
}
