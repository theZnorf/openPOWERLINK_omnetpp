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

#include "SdoUdp.h"
#include "interface/OplkSdoUdp.h"

USING_NAMESPACE

Define_Module(SdoUdp);

void SdoUdp::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkSdoUdp::Instance numberOfInstances = par("numberOfInstances");

    // init stub
    interface::OplkSdoUdp::setLibraryInfo(libName, numberOfInstances);
    interface::OplkSdoUdp::getInstance().initModule(this);
}

void SdoUdp::handleMessage(cMessage *msg)
{
}

void SdoUdp::createSocket(ConnectionType* connection)
{
    Enter_Method(__FUNCTION__);
}

void SdoUdp::closeSocket()
{
    Enter_Method(__FUNCTION__);
}

void SdoUdp::sendToSocket(ConnectionType* connection, FrameType* data, SizeType size)
{
    Enter_Method(__FUNCTION__);
}

void SdoUdp::ciricalSection(OPLK::BoolType enable)
{
    Enter_Method(__FUNCTION__);
}
