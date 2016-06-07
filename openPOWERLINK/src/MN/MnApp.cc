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

#include <MnApp.h>
#include "MsgPtr.h"
#include "ApiDef.h"
#include "InitMessage_m.h"
#include "OplkException.h"


USING_NAMESPACE

Define_Module(MnApp)


MnApp::MnApp()
{
}

MnApp::~MnApp()
{
}

interface::api::ErrorType MnApp::initApp()
{
    interface::api::ErrorType ret = interface::api::Error::kErrorOk;
    try
    {
        mCnt = 0;

        for (auto i = 0u; (i < cMaxNodes) && (mUsedNodeIds[i] != 0); i++)
        {
            mNodeVar[i].leds = 0;
            mNodeVar[i].ledsOld = 0;
            mNodeVar[i].input = 0;
            mNodeVar[i].inputOld = 0;
            mNodeVar[i].toggle = 0;
            mNodeVar[i].period = 0;
        }

        EV << "Initializing process image..." << std::endl;
        EV << "Size of process image: Input = " << (ULONG) sizeof(PI_IN) << " Output = " << (ULONG) sizeof(PI_OUT)
                << std::endl;

        allocProcessImage(sizeof(PI_IN), sizeof(PI_OUT));

        mProcessImageIn = (PI_IN*) getProcessImageIn();
        mProcessImageOut = (PI_OUT*) getProcessImageOut();

        if ((mProcessImageIn == nullptr) || (mProcessImageOut == nullptr))
            throw interface::OplkException("Invalid process image returned", interface::api::Error::kErrorNoResource);

        setupProcessImage();

    }
    catch (interface::OplkException const & e)
    {
        EV << "Stack Error: " << e.what() << std::endl;
        ret = e.errorNumber();
    }

    return ret;
}

interface::api::ErrorType MnApp::processSync()
{
    interface::api::ErrorType ret = interface::api::Error::kErrorOk;
    try
    {
        waitSyncEvent(100000);

        exchangeProcessImageOut();

        mCnt++;

        mNodeVar[0].input = mProcessImageOut->CN1_M00_DigitalInput_00h_AU8_DigitalInput;
        mNodeVar[1].input = mProcessImageOut->CN32_M00_DigitalInput_00h_AU8_DigitalInput;
        mNodeVar[2].input = mProcessImageOut->CN110_M00_DigitalInput_00h_AU8_DigitalInput;

        for (auto i = 0u; (i < cMaxNodes) && (mUsedNodeIds[i] != 0); i++)
        {
            /* Running LEDs */
            /* period for LED flashing determined by inputs */
            mNodeVar[i].period = (mNodeVar[i].input == 0) ? 1 : (mNodeVar[i].input * 20);
            if (mCnt % mNodeVar[i].period == 0)
            {
                if (mNodeVar[i].leds == 0x00)
                {
                    mNodeVar[i].leds = 0x1;
                    mNodeVar[i].toggle = 1;
                }
                else
                {
                    if (mNodeVar[i].toggle)
                    {
                        mNodeVar[i].leds <<= 1;
                        if (mNodeVar[i].leds == cAppLedMask)
                        {
                            mNodeVar[i].toggle = 0;
                        }
                    }
                    else
                    {
                        mNodeVar[i].leds >>= 1;
                        if (mNodeVar[i].leds == 0x01)
                        {
                            mNodeVar[i].toggle = 1;
                        }
                    }
                }
            }

            if (mNodeVar[i].input != mNodeVar[i].inputOld)
            {
                mNodeVar[i].inputOld = mNodeVar[i].input;
            }

            if (mNodeVar[i].leds != mNodeVar[i].ledsOld)
            {
                mNodeVar[i].ledsOld = mNodeVar[i].leds;
            }
        }

        mProcessImageIn->CN1_M00_DigitalOutput_00h_AU8_DigitalOutput = mNodeVar[0].leds;
        mProcessImageIn->CN32_M00_DigitalOutput_00h_AU8_DigitalOutput = mNodeVar[1].leds;
        mProcessImageIn->CN110_M00_DigitalOutput_00h_AU8_DigitalOutput = mNodeVar[2].leds;

        exchangeProcessImageIn();
    }
    catch (interface::OplkException const & e)
    {
        EV << "Stack Error: " << e.what() << std::endl;
        ret = e.errorNumber();
    }
    return ret;
}

void MnApp::shutdownApp()
{
    freeProcessImage();
    EV << "Stack shutdown - free process image suceeded" << std::endl;
}
