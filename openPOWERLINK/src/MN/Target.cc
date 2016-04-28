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

#include "Target.h"

USING_NAMESPACE

Define_Module(Target);


void Target::initialize()
{
    // TODO - Generated method body
}

void Target::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void Target::msleep(MilliSecondsType milliseconds)
{
}

void Target::setIpAddress(std::string interfaceName, AddressType ipAddress, AddressType subnetMask, MtuType mtu)
{
}

void Target::setDefaultGateway(AddressType defaultGateway)
{
}

Target::TickType Target::getTickCount()
{
    return static_cast<TickType>(simTime().raw());
}

void Target::setLed(LedType ledType, bool ledOn)
{
}
