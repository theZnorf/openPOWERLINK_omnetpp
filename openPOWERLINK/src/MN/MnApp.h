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

#ifndef MNAPP_H_
#define MNAPP_H_

#include <AppBase.h>
#include "xap.h"

class MnApp : public AppBase
{
        // Definitions
    private:
        static constexpr size_t cDefaultMaxCycleCount = 20;
        static constexpr size_t cAppLedCount = 8;
        static constexpr unsigned int cAppLedMask = (1 << (cAppLedCount - 1));
        static constexpr size_t cMaxNodes = 255;
        static constexpr size_t cUsedNodes = 4;

    struct AppNodeVar
    {
            UINT leds;
            UINT ledsOld;
            UINT input;
            UINT inputOld;
            UINT period;
            int toggle;
    };

        // C-Tor / D-Tor
    public:
        MnApp();
        virtual ~MnApp();

        // Methods
    protected:
        virtual interface::api::ErrorType initApp();
        virtual interface::api::ErrorType processSync();
        virtual void shutdownApp();

        // Member
    private:

        int mUsedNodeIds[cUsedNodes] = { 1, 32, 110, 0 };
        unsigned int mCnt;
        AppNodeVar mNodeVar[cMaxNodes];
        PI_IN* mProcessImageIn;
        PI_OUT* mProcessImageOut;

};

#endif /* MNAPP_H_ */
