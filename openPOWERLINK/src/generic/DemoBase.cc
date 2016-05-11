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
#include "DemoBase.h"
#include "MsgPtr.h"
#include "stack/Api.h"
#include "InitMessage_m.h"
#include "StringMessage_m.h"
#include "AppBase.h"

using namespace std;
USING_NAMESPACE

Define_Module(DemoBase);

DemoBase::DemoBase() :
        UseApiBase("apiCall")
{
}

void DemoBase::initialize()
{
    UseApiBase::initialize();

    // resolve gates
    mApiCallGate = gate("apiCall");
    mAppCallGate = gate("appCall");
    mAppApiReturnGate = gate("appApiReturn");

    // init dispatcher
    mDispatcher.registerFunction(gate("apiReturn"), std::bind(&DemoBase::processApiReturn, this, placeholders::_1));
    mDispatcher.registerFunction(gate("appReturn"), std::bind(&DemoBase::processAppReturn, this, placeholders::_1));
    mDispatcher.registerFunction(gate("appApiCall"), std::bind(&DemoBase::processAppApiCall, this, placeholders::_1));

    // schedule init message
    scheduleAt(simTime() + simtime_t::parse("1s"), new cMessage());
}

void DemoBase::handleOtherMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        // check if external message
        if (!msg->isSelfMessage())
        {
            mDispatcher.dispatch(msg.get());
        }
        else // self schedule init message
        {
            // init demo
            initPowerlink();
            initApp();
        }
    }
}

void DemoBase::initPowerlink()
{
    BYTE macAddr[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0x00 };
    char cdcFile[] = "mnobd.cdc";

    static interface::api::ApiInitParam initParam;
    static char devName[128] = { 0 };

    EV << "Initializing openPOWERLINK stack..." << std::endl;

    memset(&initParam, 0, sizeof(initParam));
    initParam.sizeOfInitParam = sizeof(initParam);

    // pass selected device name to Edrv
    initParam.hwParam.pDevName = devName;
    initParam.nodeId = 0xF0;
    initParam.ipAddress = (0xFFFFFF00 & 0xc0a86401) | initParam.nodeId;

    /* write 00:00:00:00:00:00 to MAC address, so that the driver uses the real hardware address */
    memcpy(initParam.aMacAddress, macAddr, sizeof(initParam.aMacAddress));

    initParam.fAsyncOnly = FALSE;
    initParam.featureFlags = UINT_MAX;
    initParam.cycleLen = UINT_MAX;       // required for error detection
    initParam.isochrTxMaxPayload = 256;              // const
    initParam.isochrRxMaxPayload = 256;              // const
    initParam.presMaxLatency = 50000;            // const; only required for IdentRes
    initParam.preqActPayloadLimit = 36;               // required for initialisation (+28 bytes)
    initParam.presActPayloadLimit = 36;               // required for initialisation of Pres frame (+28 bytes)
    initParam.asndMaxLatency = 150000;           // const; only required for IdentRes
    initParam.multiplCylceCnt = 0;                // required for error detection
    initParam.asyncMtu = 1500;             // required to set up max frame size
    initParam.prescaler = 2;                // required for sync
    initParam.lossOfFrameTolerance = 500000;
    initParam.asyncSlotTimeout = 3000000;
    initParam.waitSocPreq = 1000;
    initParam.deviceType = UINT_MAX;         // NMT_DeviceType_U32
    initParam.vendorId = UINT_MAX;         // NMT_IdentityObject_REC.VendorId_U32
    initParam.productCode = UINT_MAX;         // NMT_IdentityObject_REC.ProductCode_U32
    initParam.revisionNumber = UINT_MAX;         // NMT_IdentityObject_REC.RevisionNo_U32
    initParam.serialNumber = UINT_MAX;         // NMT_IdentityObject_REC.SerialNo_U32

    initParam.subnetMask = 0xFFFFFF00;
    initParam.defaultGateway = 0xC0A864FE;
    sprintf((char*) initParam.sHostname, "%02x-%08x", initParam.nodeId, initParam.vendorId);
    initParam.syncNodeId = C_ADR_SYNC_ON_SOA;
    initParam.fSyncOnPrcNode = FALSE;

    // set callback functions to null for seperation of modules
    // (events will be transmmitted via messages)
    initParam.pfnCbEvent = nullptr;

#if defined(CONFIG_KERNELSTACK_DIRECTLINK)
    initParam.pfnCbSync = processSync;
#else
    initParam.pfnCbSync = NULL;
#endif

    // initialize POWERLINK stack
    initStack();
    //auto initMessage = new cMessage("Init Stack", static_cast<short>(Api::ApiCallType::init));
    //send(initMessage, mApiCallGate);

    createStack(initParam);
    //auto initMsg = new oplkMessages::InitMessage("Create Stack", static_cast<short>(Api::ApiCallType::create));
    //initMsg->setInitParam(initParam);
    //send(initMsg, mApiCallGate);

    setCdcFilename(cdcFile);
    //auto cdcMsg = new oplkMessages::StringMessage("Set cdc file", static_cast<short>(Api::ApiCallType::setCdcFilename));
    //cdcMsg->setString(cdcFile);
    //send(cdcMsg, mApiCallGate);

    EV << "Initialization succeeded" << std::endl;
}

void DemoBase::initApp()
{
    // create init message for app module
    auto msg = new cMessage();
    msg->setKind(static_cast<short>(AppBase::AppCallType::init));

    send(msg, mAppCallGate);
}

void DemoBase::processApiReturn(RawMessagePtr msg)
{
    // forward unexpected api returns to app module
    send(msg->dup(), mAppApiReturnGate);
}

void DemoBase::processAppReturn(RawMessagePtr msg)
{
    EV << "App call returned";

    auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg);
    if (retMsg != nullptr)
        EV << " with " << retMsg->getReturnValue();
    EV << std::endl;
}

void DemoBase::processAppApiCall(RawMessagePtr msg)
{
    // forward api calls to stack
    send(msg->dup(), mApiCallGate);
}

void DemoBase::processStackShutdown()
{
    EV << "Stack is shutting down" << endl;

    // TODO: shutdown demo

}
