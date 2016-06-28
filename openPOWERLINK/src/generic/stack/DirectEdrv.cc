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

#include <sstream>
#include "DirectEdrv.h"
#include "interface/OplkException.h"
#include "interface/OplkEdrv.h"
#include "BufferMessage_m.h"
#include "OplkMessage_m.h"
#include "MsgPtr.h"
#include "ApiDef.h"
#include "debugstr.h"

using namespace std;
USING_NAMESPACE

Define_Module(DirectEdrv);

void DirectEdrv::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkEdrv::Instance numberOfInstances = par("numberOfInstances");

    // init stub
    interface::OplkEdrv::setLibraryInfo(libName, numberOfInstances);
    interface::OplkEdrv::getInstance().initModule(this);

    // resolve gates
    mEthernetOutGate = gate("ethernetOut");

    // resolve signals
    mSentEtherTypeSignal = registerSignal("sentEtherType");
    mReceivedEtherTypeSignal = registerSignal("receivedEtherType");
    mSentOplkTypeSignal = registerSignal("sentOplkMessageType");
    mReceivedOplkTypeSignal = registerSignal("receivedOplkMessageType");
}

void DirectEdrv::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if ((msg != nullptr) && mInitialized)
    {
        // cast message
        auto buffMsg = dynamic_cast<oplkMessages::BufferMessage*>(msg.get());

        if (buffMsg != nullptr)
        {
            bubble("Frame received");

            // create rx buffer
            RxBufferPtr rxBuffer(new RxBuffer(), deleteRxBuffer);
            rxBuffer->rxFrameSize = buffMsg->getBufferArraySize();
            rxBuffer->pBuffer = new UINT8[rxBuffer->rxFrameSize];
            for (auto i = 0u; i < rxBuffer->rxFrameSize; i++)
                rxBuffer->pBuffer[i] = buffMsg->getBuffer(i);
            rxBuffer->pRxTimeStamp = nullptr; //getCurrentTimestamp();

            // emit signals
            auto etherType = *((unsigned short*)(&rxBuffer->pBuffer[cEtherTypePos]));
            emit(mReceivedEtherTypeSignal, etherType);
            if (etherType == cOplKEtherType)
                emit(mReceivedOplkTypeSignal, rxBuffer->pBuffer[cOplkTypePos]);

            bubble("Frame received");

            // forward received frame
            auto ret = mRxCallback(rxBuffer.get());

            // check return
            if (ret == ReleaseRxBuffer::kEdrvReleaseRxBufferLater)
            {
                // add rx buffer ptr to internal cont
                mRxBufferCont.push_back(rxBuffer);
            }
        }
    }
}

void DirectEdrv::initEdrv(EdrvInitParamType* initParam)
{
    Enter_Method(__FUNCTION__);

    // check parameter
    if (initParam == nullptr)
        throw interface::OplkException("invalid init param", interface::api::Error::kErrorEdrvInvalidParam);

    bubble("Ethernet driver initialized");
    // init member

    // copy mac addr
    mMac[0] = initParam->aMacAddr[0];

    for (auto i = 1u; i < mMac.size(); i++)
    {
        mMac[i] = initParam->aMacAddr[i];
    }

    // set rx handler
    mRxCallback = initParam->pfnRxHandler;

    // save hw info
    mHwInfo = initParam->hwParam;

    setInitialized(true);
    refreshDisplay();
}

void DirectEdrv::exitEdrv()
{
    Enter_Method(__FUNCTION__);
    setInitialized(false);
}

DirectEdrv::MacType DirectEdrv::getMacAddr()
{
    Enter_Method(__FUNCTION__);
    return mMac.data();
}

void DirectEdrv::sendTxBuffer(TxBufferType* txBuffer)
{
    Enter_Method(__FUNCTION__);
    // check parameter
    if (txBuffer == nullptr)
        throw interface::OplkException("invalid tx buffer", interface::api::Error::kErrorEdrvInvalidParam);

    bubble("Send Tx given buffer");

    // save simulation ctx
    auto oldCtx = simulation.getContext();
    simulation.setContext(this);

    oplkMessages::BufferMessage* msg = nullptr;

    // check ethertype and emit signals
    auto etherType = *((unsigned short*)(&txBuffer->pBuffer[cEtherTypePos]));
    emit(mSentEtherTypeSignal, etherType);

    if (etherType == cOplKEtherType)
    {
        auto msgType = txBuffer->pBuffer[cOplkTypePos];

        emit(mSentOplkTypeSignal, msgType);

        // create oplk message
        auto oplkMsg = new oplkMessages::OplkMessage();

        // set message type
        oplkMsg->setMessageType(msgType);
        // set message type string
        oplkMsg->setMessageTypeStr(interface::debug::getMessageTypeStr(msgType));
        oplkMsg->setName(interface::debug::getMessageTypeStr(msgType));

        msg = oplkMsg;
    }
    else
        // create buffer message
        msg = new oplkMessages::BufferMessage();

    // fill message
    msg->setBufferArraySize(txBuffer->txFrameSize);
    for (auto i = 0u; i < txBuffer->txFrameSize; i++)
        msg->setBuffer(i, txBuffer->pBuffer[i]);


    // send message
    send(msg, mEthernetOutGate);

    // restore ctx
    simulation.setContext(oldCtx);

    // call tx handler
    if (txBuffer->pfnTxHandler != nullptr)
        txBuffer->pfnTxHandler(txBuffer);
}

void DirectEdrv::allocTxBuffer(TxBufferType* txBuffer)
{
    Enter_Method(__FUNCTION__);
    // check parameter
    if (txBuffer == nullptr)
        throw interface::OplkException("invalid tx buffer", interface::api::Error::kErrorEdrvInvalidParam);

    // allocate buffer with new
    txBuffer->pBuffer = new UINT8[txBuffer->maxBufferSize];
    if (txBuffer->pBuffer == nullptr)
        throw interface::OplkException("error allocating tx buffer ", OPLK::kErrorEdrvNoFreeBufEntry);

    txBuffer->txBufferNumber.pArg = nullptr;
}

void DirectEdrv::freeTxBuffer(TxBufferType* txBuffer)
{
    Enter_Method(__FUNCTION__);
    if (txBuffer->pBuffer != nullptr)
    {
        delete[] txBuffer->pBuffer;
        txBuffer->pBuffer = nullptr;
    }
}

void DirectEdrv::changeRxFilter(FilterType* filter, unsigned int count, unsigned int entryChanged, unsigned int changeFlags)
{
    Enter_Method(__FUNCTION__);
    // check parameter
    if (filter == nullptr)
        throw interface::OplkException("invalid filter", interface::api::Error::kErrorEdrvInvalidParam);

    bubble("change filter");
}

void DirectEdrv::clearRxMulticastMacAddr(MacType macAddr)
{
    Enter_Method(__FUNCTION__);
    bubble("clear rx multicast mac addr");
}

void DirectEdrv::setRxMulticastMacAddr(MacType macAddr)
{
    Enter_Method(__FUNCTION__);
    bubble("set rx multicast mac addr");
}

void DirectEdrv::refreshDisplay()
{
    if (mInitialized)
    {
        stringstream strStream;
        strStream << " MacAddress: " << std::hex << (int) mMac[0];
        for (auto i = 1u; i < mMac.size(); i++)
            strStream << ":" << std::hex << (int) mMac[i];
        strStream << std::endl;
        strStream << " Device Number: " << mHwInfo.devNum << std::endl;
        strStream << " Device Name: " << mHwInfo.pDevName << std::endl;

        getDisplayString().setTagArg("t", 0, strStream.str().c_str());
        getDisplayString().setTagArg("i", 1, "green");
    }
    else
    {
        getDisplayString().setTagArg("t", 0, "Uninitialized");
        getDisplayString().setTagArg("i", 1, "red");
    }
}

void DirectEdrv::setInitialized(bool initialized)
{
    mInitialized = initialized;

    refreshDisplay();
}

DirectEdrv::Timestamp* DirectEdrv::getCurrentTimestamp()
{
    return new Timestamp(simTime().inUnit(SimTimeUnit::SIMTIME_NS));
}

void DirectEdrv::deleteRxBuffer(RxBuffer* rxBuffer)
{
    if (rxBuffer != nullptr)
    {
        if (rxBuffer->pBuffer != nullptr)
        {
            delete[] rxBuffer->pBuffer;
            rxBuffer->pBuffer = nullptr;
        }
        if (rxBuffer->pRxTimeStamp != nullptr)
        {
            delete rxBuffer->pRxTimeStamp;
            rxBuffer->pRxTimeStamp = nullptr;
        }
        delete rxBuffer;
    }
}
