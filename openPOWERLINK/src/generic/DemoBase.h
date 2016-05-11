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

#ifndef __OPENPOWERLINK_DEMOBASE_H_
#define __OPENPOWERLINK_DEMOBASE_H_

#include <omnetpp.h>
#include "UseApiBase.h"
#include "ReturnMessage_m.h"
#include "MessageDispatcher.h"


class DemoBase : public UseApiBase
{
        // Definitions
    private:
        using RawMessagePtr = MessageDispatcher::RawMessagePtr;

        enum class DemoState : short
        {
                initializing,
                mainloop
        };

        // C-Tor
    public:
        DemoBase();

        // Methods
    protected:
        virtual void initialize();
        virtual void handleOtherMessage(MessagePtr msg);

    private:
        void initPowerlink();
        void initApp();

        void processApiReturn(RawMessagePtr msg);
        void processAppReturn(RawMessagePtr msg);
        void processAppApiCall(RawMessagePtr msg);
        void processStackShutdown();

        // Member
    private:
        OPP::cGate* mApiCallGate;
        OPP::cGate* mAppCallGate;
        OPP::cGate* mAppApiReturnGate;
        MessageDispatcher mDispatcher;
};

#endif
