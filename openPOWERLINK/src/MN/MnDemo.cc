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

#include "MnDemo.h"
#include <cstring>

USING_NAMESPACE

Define_Module(MnDemo);

void MnDemo::initialize()
{
    // call base method
    DemoBase::initialize();

    // resolve parameter
    auto cdcPar = par("cdcFilename").stringValue();
    CharPtr cdcFile(new char[std::strlen(cdcPar)]);
    std::strcpy(cdcFile.get(), cdcPar);
    mCdcFileName = std::move(cdcFile);
}

void MnDemo::initPowerlink()
{

    static BYTE macAddr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
    initParam.pfnCbEvent = nullptr; // events will be forwarded to the EventBase module
    initParam.pfnCbSync = nullptr; // process sync calls will be forwarded to the AppBase module

    // initialize POWERLINK stack
    initStack();
    createStack(initParam);
    setCdcFilename(mCdcFileName.get());

    EV << "Initialization succeeded" << std::endl;
}

void MnDemo::shutdownPowerlink()
{
    EV << "Shutdown openPOWERLINK" << std::endl;
    execNmtCommand(interface::api::NmtEventType::kNmtEventSwitchOff);
    destroyStack();
}
