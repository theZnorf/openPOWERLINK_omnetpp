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
#include "interface/OplkException.h"
#include "interface/OplkEdrv.h"
#include <sstream>

using namespace std;
USING_NAMESPACE

Define_Module(Edrv);

void Edrv::initialize()
{
    // init stub
    interface::OplkEdrv::getInstance().initModule(this);

    // resolve gate
    mEthernetInGate = gate("ethernetIn");
    mEthernetOutGate = gate("ethernetOut");
}

void Edrv::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}


void Edrv::finish()
{

}

void Edrv::initEdrv(EdrvInitParamType* initParam)
{
    bubble("Ethernet driver initialized");
    // init member
    stringstream strStream;

    // copy mac addr
    mMac[0] = initParam->aMacAddr[0];
    strStream << "MacAddress: " << std::hex << (int)mMac[0];

    for (auto i = 1u; i < mMac.size(); i++)
    {
        mMac[i] = initParam->aMacAddr[i];
        strStream << ":" << (int)mMac[i];
    }
    strStream << std::endl;

    // set rx handler
    mRxCallback = initParam->pfnRxHandler;

    // save hw info
    mHwInfo = initParam->hwParam;

    strStream << "Device Number: " << mHwInfo.devNum << std::endl;
    strStream << "Device Name: " << mHwInfo.pDevName << std::endl;

    getDisplayString().setTagArg("t", 0, strStream.str().c_str());
    getDisplayString().setTagArg("i",1,"green");
}

void Edrv::exitEdrv()
{
    getDisplayString().setTagArg("t", 0, "Uninitialized");
    getDisplayString().setTagArg("i",1,"red");
}

Edrv::MacType Edrv::getMacAddr()
{
    return mMac.data();
}

void Edrv::sendTxBuffer(TxBufferType* txBuffer)
{
    bubble("Send Tx given buffer");

    // call tx handler
    txBuffer->pfnTxHandler(txBuffer);
}

void Edrv::allocTxBuffer(TxBufferType* txBuffer)
{
    // allocate buffer with new
    txBuffer->pBuffer = new UINT8[txBuffer->maxBufferSize];
    if (txBuffer->pBuffer == nullptr)
        throw interface::OplkException("error allocating tx buffer ", OPLK::kErrorEdrvNoFreeBufEntry);

    txBuffer->txBufferNumber.pArg = nullptr;
}

void Edrv::freeTxBuffer(TxBufferType* txBuffer)
{
    if (txBuffer->pBuffer != nullptr)
    {
        delete[] txBuffer->pBuffer;
        txBuffer->pBuffer = nullptr;
    }
}

void Edrv::changeRxFilter(FilterType* filter, unsigned int count, unsigned int entryChanged, unsigned int changeFlags)
{
    bubble("change filter");
}

void Edrv::clearRxMulticastMacAddr(MacType macAddr)
{
    bubble("clear rx multicast mac addr");
}

void Edrv::setRxMulticastMacAddr(MacType macAddr)
{
    bubble("set rx multicast mac addr");
}
