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

USING_NAMESPACE
using namespace std;

Define_Module(CnApp);

/* structure for input process image */
typedef struct
{
   BYTE    digitalIn;
} PI_IN;

/* structure for output process image */
typedef struct
{
   BYTE    digitalOut;
} PI_OUT;

/* process image */
static PI_IN*   pProcessImageIn_l;
static PI_OUT*  pProcessImageOut_l;

/* application variables */
static BYTE    digitalIn_g;                 // 8 bit digital input
static BYTE    digitalOut_g;                // 8 bit digital output

interface::api::ErrorType CnApp::initApp()
{
    try
    {
        /* Allocate process image */
        EV << "Initializing process image..." << endl;
        EV << "Size of process image: Input = " << sizeof(PI_IN) << " Output = " << sizeof(PI_OUT) << endl;

        allocProcessImage(sizeof(PI_IN), sizeof(PI_OUT));

        pProcessImageIn_l = (PI_IN*)getProcessImageIn();
        pProcessImageOut_l = (PI_OUT*)getProcessImageOut();

        /* link process variables used by CN to object dictionary */
        EV <<  "Linking process image vars:" << endl;

        auto obdSize = sizeof(pProcessImageIn_l->digitalIn);
        auto varEntries = 1u;
        linkProcessImageObject(0x6000, 0x01, offsetof(PI_IN, digitalIn),
                                          FALSE, obdSize, &varEntries);

        obdSize = sizeof(pProcessImageOut_l->digitalOut);
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
        waitSyncEvent(100000);

        exchangeProcessImageOut();

        /* read input image - digital outputs */
        digitalOut_g = pProcessImageOut_l->digitalOut;

        /* setup output image - digital inputs */
        pProcessImageIn_l->digitalIn = digitalIn_g;

        exchangeProcessImageIn();

    }
    catch (interface::OplkException const & e)
    {
        return e.errorNumber();
    }
    return interface::api::Error::kErrorOk;
}

void CnApp::shutdownApp()
{
    freeProcessImage();
    EV << "Stack shutdown - free process image suceeded" << std::endl;
}
