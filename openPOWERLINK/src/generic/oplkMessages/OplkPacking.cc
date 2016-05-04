/*
 * OplkPacking.cc
 *
 *  Created on: May 3, 2016
 *      Author: franz
 */

#include <OplkPacking.h>

namespace oplkMessages
{
    void doUnpacking(OPP::cCommBuffer *b, PointerCont& cont)
    {
        b->unpack(cont);
    }

    void doPacking(OPP::cCommBuffer *b, PointerCont& cont)
    {
        b->pack(cont);
    }

    void doUnpacking(OPP::cCommBuffer *b, interface::api::ApiInitParam& param)
    {
        DEBUG_TRAP;
        b->unpack(param.sizeOfInitParam);
        b->unpack(param.fAsyncOnly);
        b->unpack(param.nodeId);
        b->unpack(param.aMacAddress, 6);
        b->unpack(param.featureFlags);
        b->unpack(param.cycleLen);
        b->unpack(param.isochrTxMaxPayload);
        b->unpack(param.isochrRxMaxPayload);
        b->unpack(param.presMaxLatency);
        b->unpack(param.preqActPayloadLimit);
        b->unpack(param.presActPayloadLimit);
        b->unpack(param.asndMaxLatency);
        b->unpack(param.multiplCylceCnt);
        b->unpack(param.asyncMtu);
        b->unpack(param.prescaler);
        b->unpack(param.lossOfFrameTolerance);
        b->unpack(param.waitSocPreq);
        b->unpack(param.asyncSlotTimeout);
        b->unpack(param.deviceType);
        b->unpack(param.vendorId);
        b->unpack(param.productCode);
        b->unpack(param.revisionNumber);
        b->unpack(param.serialNumber);
        b->unpack(param.vendorSpecificExt1);
        b->unpack(param.verifyConfigurationDate);
        b->unpack(param.verifyConfigurationTime);
        b->unpack(param.applicationSwDate);
        b->unpack(param.applicationSwTime);
        b->unpack(param.ipAddress);
        b->unpack(param.subnetMask);
        b->unpack(param.defaultGateway);
        b->unpack(param.sHostname, 32);
        b->unpack(param.aVendorSpecificExt2, 48);
        b->unpack(param.pDevName);
        b->unpack(param.pHwVersion);
        b->unpack(param.pSwVersion);
        auto eventCb = (PointerCont) param.pfnCbEvent;
        b->unpack(eventCb);
        auto userArg = (PointerCont) param.pEventUserArg;
        b->unpack(userArg);
        auto cbSync = (PointerCont) param.pfnCbSync;
        b->unpack(cbSync);
        doUnpacking(b, param.hwParam);
        b->unpack(param.syncResLatency);
        b->unpack(param.syncNodeId);
        b->unpack(param.fSyncOnPrcNode);
        b->unpack(param.minSyncTime);
        b->unpack(param.sdoStackType);
    }

    void doPacking(OPP::cCommBuffer *b, interface::api::ApiInitParam& param)
    {
        DEBUG_TRAP;
        b->pack(param.sizeOfInitParam);
        b->pack(param.fAsyncOnly);
        b->pack(param.nodeId);
        b->pack(param.aMacAddress, 6);
        b->pack(param.featureFlags);
        b->pack(param.cycleLen);
        b->pack(param.isochrTxMaxPayload);
        b->pack(param.isochrRxMaxPayload);
        b->pack(param.presMaxLatency);
        b->pack(param.preqActPayloadLimit);
        b->pack(param.presActPayloadLimit);
        b->pack(param.asndMaxLatency);
        b->pack(param.multiplCylceCnt);
        b->pack(param.asyncMtu);
        b->pack(param.prescaler);
        b->pack(param.lossOfFrameTolerance);
        b->pack(param.waitSocPreq);
        b->pack(param.asyncSlotTimeout);
        b->pack(param.deviceType);
        b->pack(param.vendorId);
        b->pack(param.productCode);
        b->pack(param.revisionNumber);
        b->pack(param.serialNumber);
        b->pack(param.vendorSpecificExt1);
        b->pack(param.verifyConfigurationDate);
        b->pack(param.verifyConfigurationTime);
        b->pack(param.applicationSwDate);
        b->pack(param.applicationSwTime);
        b->pack(param.ipAddress);
        b->pack(param.subnetMask);
        b->pack(param.defaultGateway);
        b->pack(param.sHostname, 32);
        b->pack(param.aVendorSpecificExt2, 48);
        b->pack(param.pDevName);
        b->pack(param.pHwVersion);
        b->pack(param.pSwVersion);

        PointerCont cbEvent = 0;
        PointerCont userArg = 0;
        PointerCont cbSync = 0;

        b->pack(cbEvent);
        b->pack(userArg);
        b->pack(cbSync);

        param.pfnCbEvent = reinterpret_cast<decltype(param.pfnCbEvent)>(cbEvent);
        param.pEventUserArg = reinterpret_cast<decltype(param.pEventUserArg)>(userArg);
        param.pfnCbSync = reinterpret_cast<decltype(param.pfnCbSync)>(cbSync);


        doPacking(b, param.hwParam);
        b->pack(param.syncResLatency);
        b->pack(param.syncNodeId);
        b->pack(param.fSyncOnPrcNode);
        b->pack(param.minSyncTime);
        b->pack(param.sdoStackType);
    }

    void doUnpacking(OPP::cCommBuffer* b, interface::api::ApiEventArg& arg)
    {
        PointerCont userArg;

        b->unpack(userArg);
        arg.pUserArg = reinterpret_cast<decltype(arg.pUserArg)>(userArg);
    }

    void doPacking(OPP::cCommBuffer* b, interface::api::ApiEventArg& arg)
    {
        auto userArg = (PointerCont) arg.pUserArg;

        b->pack(userArg);
    }

    void doUnpacking(OPP::cCommBuffer* b, interface::api::HwParam& param)
    {
        b->unpack(param.devNum);
        b->unpack(param.pDevName);
    }

    void doPacking(OPP::cCommBuffer* b, interface::api::HwParam& param)
    {
        b->pack(param.devNum);
        b->pack(param.pDevName);
    }

    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdInitParam& param)
    {
        b->unpack(param.numGeneric);
        doUnpacking(b, param.pGenericPart, param.numGeneric);
        b->unpack(param.numManufacturer);
        doUnpacking(b, param.pManufacturerPart, param.numManufacturer);
        b->unpack(param.numDevice);
        doUnpacking(b, param.pDevicePart, param.numDevice);
    }

    void doPacking(OPP::cCommBuffer* b, interface::api::ObdInitParam& param)
    {
        b->pack(param.numGeneric);
        doPacking(b, param.pGenericPart, param.numGeneric);
        b->pack(param.numManufacturer);
        doPacking(b, param.pManufacturerPart, param.numManufacturer);
        b->pack(param.numDevice);
        doPacking(b, param.pDevicePart, param.numDevice);
    }

    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdEntry& entry)
    {
        b->unpack(entry.index);
        b->unpack(entry.count);
        doUnpacking(b, entry.pSubIndex, entry.count);

        PointerCont callback = 0;
        b->unpack(callback);

        entry.pfnCallback = reinterpret_cast<decltype(entry.pfnCallback)>(callback);
    }

    void doPacking(OPP::cCommBuffer* b, interface::api::ObdEntry& entry)
    {
        b->pack(entry.index);
        b->pack(entry.count);
        doPacking(b, entry.pSubIndex, entry.count);
        b->pack(entry.pfnCallback);
    }

    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdSubEntry& subentry)
    {
        b->unpack(subentry.subIndex);
        b->unpack(subentry.type);
        b->unpack(subentry.access);

        PointerCont defaultVal = 0;
        PointerCont current = 0;

        b->unpack(defaultVal);
        b->unpack(current);
        subentry.pDefault = reinterpret_cast<decltype(subentry.pDefault)>(defaultVal);
        subentry.pCurrent = reinterpret_cast<decltype(subentry.pCurrent)>(current);
    }

    void doPacking(OPP::cCommBuffer* b, interface::api::ObdSubEntry& subentry)
    {
        b->pack(subentry.subIndex);
        b->pack(subentry.type);
        b->pack(subentry.access);
        b->pack(subentry.pDefault);
        b->pack(subentry.pCurrent);
    }


    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdCallbackParam& param)
    {
        b->unpack(param.obdEvent);
        b->unpack(param.index);
        b->unpack(param.subIndex);
        PointerCont arg;
        b->unpack(arg);
        param.pArg = reinterpret_cast<decltype(param.pArg)>(arg);
        b->unpack(param.abortCode);
    }

    void doPacking(OPP::cCommBuffer* b, interface::api::ObdCallbackParam& param)
    {
        b->pack(param.obdEvent);
        b->pack(param.index);
        b->pack(param.subIndex);
        b->pack(param.pArg);
        b->pack(param.abortCode);
    }


} /* namespace oplkMessages */
