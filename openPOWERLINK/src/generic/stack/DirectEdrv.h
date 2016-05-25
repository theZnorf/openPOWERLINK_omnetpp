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
#include <vector>
#include <memory>
#include <omnetpp.h>
#include "interface/OplkEdrv.h"

class DirectEdrv : public OPP::cSimpleModule
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
        using RxBuffer = interface::OplkEdrv::RxBufferType;
        using RxBufferPtr = std::shared_ptr<RxBuffer>;
        using RxBufferCont = std::vector<RxBufferPtr>;
        using Timestamp = unsigned int;
        using ReleaseRxBuffer = OPLK::eEdrvReleaseRxBuffer;

        // Methods
    protected:
        virtual void initialize();
        virtual void handleMessage(OPP::cMessage *rawMsg);

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

    private:
        void refreshDisplay();
        void setInitialized(bool initialized);
        Timestamp* getCurrentTimestamp();

        // static Methods
    public:
        static void deleteRxBuffer(RxBuffer* rxBuffer);

        // Member
    private:
        MacCont mMac;
        RxCallbackType mRxCallback;
        HwInfoType mHwInfo;
        bool mInitialized;
        OPP::cGate* mEthernetOutGate;
        OPP::cGate* mEthernetInGate;
        RxBufferCont mRxBufferCont;

        OPP::simsignal_t mSentEtherTypeSignal;
        OPP::simsignal_t mReceivedEtherTypeSignal;
        OPP::simsignal_t mSentOplkTypeSignal;
        OPP::simsignal_t mReceivedOplkTypeSignal;

        size_t const cEtherTypePos = 12;
        size_t const cOplkTypePos = 14;
        unsigned short cOplKEtherType = 0xAB88;
};

#endif
