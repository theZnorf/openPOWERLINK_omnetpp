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
#include "OplkException.h"
#include "MsgPtr.h"
#include "UdpMessage_m.h"
#include <sstream>

USING_NAMESPACE

Define_Module(SdoUdp);

void SdoUdp::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkSdoUdp::Instance numberOfInstances = par(
            "numberOfInstances");

    // init stub
    interface::OplkSdoUdp::setLibraryInfo(libName, numberOfInstances);
    interface::OplkSdoUdp::getInstance().initModule(this);

    // resolve gate
    mSendGate = gate("udpIn");
}

void SdoUdp::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (rawMsg != nullptr)
    {
        // cast message
        auto udpMsg = dynamic_cast<oplkMessages::UdpMessage*>(msg.get());

        if (udpMsg != nullptr)
        {
            // create received connection
            ConnectionType connection;
            connection.ipAddr = udpMsg->getIpAddr();
            connection.port = udpMsg->getUdpPort();

            AsySdoSeq asySdoSeq;
            // Fill structure with received data
            auto ptr = (BYTE*) &asySdoSeq;
            for (auto i = 0u;
                    (i < udpMsg->getBufferArraySize())
                            && (i < sizeof(AsySdoSeq)); i++)
            {
                ptr[i] = udpMsg->getBuffer(i);
            }

            // call interface function
            mInterfaceFunctions.receiveDataCb(&connection, &asySdoSeq,
                    udpMsg->getBufferArraySize());
        }
    }
}

void SdoUdp::createSocket(ConnectionType* connection)
{
    Enter_Method
    (__FUNCTION__);

    if (connection == nullptr)
        throw interface::OplkException("Invalid udp socket connection",
                interface::api::Error::kErrorSdoUdpInvalidHdl);

    std::stringstream strStream;
    strStream << "Open Socket (" << connection->ipAddr << " : "
            << connection->port << ")";

    getDisplayString().setTagArg("t", 0, strStream.str().c_str());
}

void SdoUdp::closeSocket()
{
    Enter_Method
    (__FUNCTION__);

    getDisplayString().setTagArg("t", 0, "");
}

void SdoUdp::sendToSocket(ConnectionType* connection, FrameType* data,
        SizeType size)
{
    Enter_Method
    (__FUNCTION__);

    if ((connection == nullptr) || (data == nullptr))
        throw interface::OplkException("Invalid parameter",
                interface::api::Error::kErrorSdoUdpInvalidHdl);

// create udp message
    auto msg = new oplkMessages::UdpMessage();

// set connection
    msg->setIpAddr(connection->ipAddr);
    msg->setUdpPort(connection->port);

// copy frame
    msg->setBufferArraySize(size);
    auto ptr = (BYTE*) &data->messageType;
    for (auto i = 0u; i < size; i++)
        msg->setBuffer(i, ptr[i]);

// send message
    send(msg, mSendGate);
}

void SdoUdp::ciricalSection(OPLK::BoolType enable)
{
    Enter_Method
    (__FUNCTION__);

// TODO: check if necessary and implement
}

SdoUdp::InterfaceFunctions* SdoUdp::getInterfaceFunctions()
{
    return &mInterfaceFunctions;
}
