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

#ifndef __OPENPOWERLINK_TARGET_H_
#define __OPENPOWERLINK_TARGET_H_

#include <omnetpp.h>
#include <string>
#include "interface/OplkTarget.h"

class Target : public OPP::cSimpleModule
{
        // Definitions
    public:
        using MilliSecondsType = interface::OplkTarget::MilliSecondsType;
        using AddressType = interface::OplkTarget::AddressType;
        using MtuType = interface::OplkTarget::MtuType;
        using TickType = interface::OplkTarget::TickType;
        using LedType = interface::OplkTarget::LedType;


        // Methods
    protected:
        virtual void initialize();
        virtual void handleMessage(OPP::cMessage *msg);

    public:
        void msleep(MilliSecondsType milliseconds);
        void setIpAddress(std::string interfaceName, AddressType ipAddress, AddressType subnetMask, MtuType mtu);
        void setDefaultGateway(AddressType defaultGateway);
        TickType getTickCount();
        void setLed(LedType ledType, bool ledOn);

    private:
        void refreshDisplay();

        // Member
    private:
        bool mStatusLed = false;
        bool mErrorLed = false;
};

#endif
