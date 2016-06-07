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

#ifndef __OPENPOWERLINK_CNAPP_H_
#define __OPENPOWERLINK_CNAPP_H_

#include <AppBase.h>

class CnApp : public AppBase
{
        // Definitions
    private:
        // structure for input process image
        struct PI_IN
        {
                BYTE digitalIn;
        };

        // structure for output process image
        struct PI_OUT
        {
                BYTE digitalOut;
        };

        enum class CnAppCallType
            : short
            {
                refreshDisplay = 10
        };

        // Methods
    protected:
        virtual interface::api::ErrorType initApp();
        virtual interface::api::ErrorType processSync();
        virtual void handleAppMessage(MessagePtr msg);
        virtual void shutdownApp();

    private:
        void refreshDisplay();

        // Member
    private:
        // process image
        PI_IN* mProcessImageIn;
        PI_OUT* mProcessImageOut;

        // application variables
        BYTE mDigitalIn;   // 8 bit digital input
        BYTE mDigitalOut;  // 8 bit digital output

};

#endif
