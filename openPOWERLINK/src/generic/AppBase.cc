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

#include "xap.h"

USING_NAMESPACE

Define_Module(AppBase);

AppBase::AppBase()
 : UseApiBase("functionCall")
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
//    int i;
//
//    cnt_l = 0;
//
//    for (i = 0; (i < MAX_NODES) && (usedNodeIds_l[i] != 0); i++)
//    {
//        nodeVar_l[i].leds = 0;
//        nodeVar_l[i].ledsOld = 0;
//        nodeVar_l[i].input = 0;
//        nodeVar_l[i].inputOld = 0;
//        nodeVar_l[i].toggle = 0;
//        nodeVar_l[i].period = 0;
//    }
//
//    printf("Initializing process image...\n");
//    printf("Size of process image: Input = %lu Output = %lu\n", (ULONG)sizeof(PI_IN), (ULONG)sizeof(PI_OUT));
//    eventlog_printMessage(kEventlogLevelInfo, kEventlogCategoryGeneric,
//                        "Allocating process image: Input:%lu Output:%lu", (ULONG)sizeof(PI_IN), (ULONG)sizeof(PI_OUT));
//
//    ret = oplk_allocProcessImage(sizeof(PI_IN), sizeof(PI_OUT));
//    if (ret != kErrorOk)
//    {
//        return ret;
//    }
//
//    pProcessImageIn_l = (PI_IN*)oplk_getProcessImageIn();
//    pProcessImageOut_l = (PI_OUT*)oplk_getProcessImageOut();
//
//    ret = oplk_setupProcessImage();

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
