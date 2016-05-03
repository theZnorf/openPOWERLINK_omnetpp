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

#ifndef __OPENPOWERLINK_EDRV_H_
#define __OPENPOWERLINK_EDRV_H_

#include <array>
#include <omnetpp.h>
#include "interface/OplkEdrv.h"

class Edrv : public OPP::cSimpleModule
{
        // Definitions
    public:
        using EdrvInitParamType = interface::OplkEdrv::EdrvInitParamType;
        using MacType = interface::OplkEdrv::MacType;
        using TxBufferType = interface::OplkEdrv::TxBufferType;
        using FilterType = interface::OplkEdrv::FilterType;
        using MacCont = std::array<UINT8, 6>;
        using RxCallbackType = interface::OplkEdrv::RxCallbackType;
        using HwInfoType = interface::OplkEdrv::HwInfoType;

        // Methods
    protected:
        virtual void initialize();
        virtual void handleMessage(OPP::cMessage *msg);

    public:
        void initEdrv(EdrvInitParamType* initParam);
        void exitEdrv();
        MacType getMacAddr();
        void sendTxBuffer(TxBufferType* txBuffer);
        void allocTxBuffer(TxBufferType* txBuffer);
        void freeTxBuffer(TxBufferType* txBuffer);
        void changeRxFilter(FilterType* filter, UINT count, UINT entryChanged, UINT changeFlags);
        void clearRxMulticastMacAddr(MacType macAddr);
        void setRxMulticastMacAddr(MacType macAddr);

        // Member
    private:
        MacCont mMac;
        RxCallbackType mRxCallback;
        HwInfoType mHwInfo;
};

#endif
