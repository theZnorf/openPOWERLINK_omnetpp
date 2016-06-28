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

#ifndef __OPENPOWERLINK_SDOUDP_H_
#define __OPENPOWERLINK_SDOUDP_H_

#include <functional>
#include <omnetpp.h>
#include "interface/OplkSdoUdp.h"
#include "ApiDef.h"

class SdoUdp : public OPP::cSimpleModule
{
        // Definitions
    public:
        using ConnectionType = interface::OplkSdoUdp::ConnectionType;
        using FrameType = interface::OplkSdoUdp::FrameType;
        using SizeType = interface::OplkSdoUdp::SizeType;
        using AsySdoSeq = interface::OplkSdoUdp::AsySdoSeq;

        using ReceiveDataCb = std::function<interface::api::ErrorType(ConnectionType*, AsySdoSeq*, unsigned int)>;

        struct InterfaceFunctions
        {
                ReceiveDataCb receiveDataCb;
        };

        // Methods
    protected:
        virtual void initialize();
        virtual void handleMessage(OPP::cMessage *msg);

    public:
        void createSocket(ConnectionType * connection);
        void closeSocket();
        void sendToSocket(ConnectionType * connection, FrameType * data,
                SizeType size);
        void ciricalSection(OPLK::BoolType enable);

        InterfaceFunctions* getInterfaceFunctions();

        // Member
    private:
        InterfaceFunctions mInterfaceFunctions;
        OPP::cGate* mSendGate;
};

#endif
