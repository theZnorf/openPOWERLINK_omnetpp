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

#include "Edrv.h"

USING_NAMESPACE

Define_Module(Edrv);

void Edrv::initialize()
{
    // TODO - Generated method body
}

void Edrv::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void Edrv::initEdrv(EdrvInitParamType* initParam)
{
}

void Edrv::exitEdrv()
{
}

Edrv::MacType Edrv::getMacAddr()
{
    return nullptr;
}

void Edrv::sendTxBuffer(TxBufferType* txBuffer)
{
}

void Edrv::allocTxBuffer(TxBufferType* txBuffer)
{
}

void Edrv::freeTxBuffer(TxBufferType* txBuffer)
{
}

void Edrv::changeRxFilter(FilterType* filter, unsigned int count, unsigned int entryChanged, unsigned int changeFlags)
{
}

void Edrv::clearRxMulticastMacAddr(MacType macAddr)
{
}

void Edrv::setRxMulticastMacAddr(MacType macAddr)
{
}
