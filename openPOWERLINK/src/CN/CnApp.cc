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

#include "CnApp.h"
#include "OplkException.h"

#include <sstream>

USING_NAMESPACE
using namespace std;

Define_Module(CnApp);

interface::api::ErrorType CnApp::initApp()
{
    try
    {
        /* Allocate process image */
        EV << "Initializing process image..." << endl;
        EV << "Size of process image: Input = " << sizeof(PI_IN) << " Output = "
                << sizeof(PI_OUT) << endl;

        allocProcessImage(sizeof(PI_IN), sizeof(PI_OUT));

        mProcessImageIn = (PI_IN*) getProcessImageIn();
        mProcessImageOut = (PI_OUT*) getProcessImageOut();

        /* link process variables used by CN to object dictionary */
        EV << "Linking process image vars:" << endl;

        auto obdSize = sizeof(mProcessImageIn->digitalIn);
        auto varEntries = 1u;
        linkProcessImageObject(0x6000, 0x01, offsetof(PI_IN, digitalIn),
        FALSE, obdSize, &varEntries);

        obdSize = sizeof(mProcessImageOut->digitalOut);
        varEntries = 1u;
        linkProcessImageObject(0x6200, 0x01, offsetof(PI_OUT, digitalOut),
        TRUE, obdSize, &varEntries);

        EV << "Linking process vars... ok" << endl << endl;

    }
    catch (interface::OplkException const & e)
    {
        return e.errorNumber();
    }
    return interface::api::Error::kErrorOk;
}

interface::api::ErrorType CnApp::processSync()
{
    try
    {
        exchangeProcessImageOut();

        /* read input image - digital outputs */
        mDigitalOut = mProcessImageOut->digitalOut;

        /* setup output image - digital inputs */
        mProcessImageIn->digitalIn = mDigitalIn;

        exchangeProcessImageIn();

        // create message with lower priority as default for achieving the execution after the response message
        auto msg = new cMessage("refresh Cn App display",
                static_cast<short>(CnAppCallType::refreshDisplay));
        msg->setSchedulingPriority(1);

        // schedule self message for refreshing display
        scheduleAt(simTime(), msg);
    }
    catch (interface::OplkException const & e)
    {
        return e.errorNumber();
    }
    return interface::api::Error::kErrorOk;
}

void CnApp::handleAppMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        // check kind
        switch (static_cast<CnAppCallType>(msg->getKind()))
        {
            case CnAppCallType::refreshDisplay:
                refreshDisplay();
                break;

            default:
                error("Invalid message type in CnApp: %d", msg->getKind());
        }
    }
}

void CnApp::shutdownApp()
{
    freeProcessImage();
    EV << "Stack shutdown - free process image suceeded" << std::endl;
}

void CnApp::refreshDisplay()
{
    std::stringstream strStream;

    strStream << "IN:  " << std::ios::hex << mDigitalIn << std::endl;
    strStream << "OUT: " << std::ios::hex << mDigitalOut << std::endl;

    getDisplayString().setTagArg("t", 0, strStream.str().c_str());

}
