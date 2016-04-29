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

#include "Api.h"
#include "interface/oplkinc.h"

using namespace std;
USING_NAMESPACE
Define_Module(Api);

static interface::api::ErrorType processEvents(interface::api::ApiEventType eventType_p,
        interface::api::ApiEventArg* pEventArg_p, void* pUserArg_p)
{
    EV << "User event:" << endl;
    EV << " EvenType: " << eventType_p << endl;

    return OPLK::kErrorOk;
}

void Api::initialize()
{
    interface::api::ApiFunctions & functions = mApi;

    // init interface
    interface::OplkApi::getInstance().initModule(&functions);

    // schedule self message
    scheduleAt(simTime() + simtime_t::parse("1s"), new cMessage());
}

void Api::handleMessage(cMessage *msg)
{
    BYTE macAddr[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0x00 };
    char cdcFile[] = "mnobd.cdc";
    interface::api::ErrorType ret = OPLK::kErrorOk;

    static interface::api::ApiInitParam initParam;
    static char devName[128];

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

    // set callback functions
    initParam.pfnCbEvent = processEvents;

#if defined(CONFIG_KERNELSTACK_DIRECTLINK)
    initParam.pfnCbSync = processSync;
#else
    initParam.pfnCbSync = NULL;
#endif

    // initialize POWERLINK stack
    ret = mApi.initialize();
    if (ret != OPLK::kErrorOk)
    {
        EV << "oplk_initialize() failed with " << ret << " : " << mApi.getRetStr(ret) << std::endl;
    }

    ret = mApi.create(&initParam);
    if (ret != OPLK::kErrorOk)
    {
        EV << "oplk_create() failed with " << ret << " : " << mApi.getRetStr(ret) << std::endl;
    }

    ret = mApi.setCdcFilename(cdcFile);
    if (ret != OPLK::kErrorOk)
    {
        EV << "oplk_setCdcFilename() failed with " << ret << " : " << mApi.getRetStr(ret) << std::endl;
    }

    EV << "Initialization resulted: " << ret << " : " << mApi.getRetStr(ret) << std::endl;
}
