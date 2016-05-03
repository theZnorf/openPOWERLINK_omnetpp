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
#include <functional>
#include <map>
#include "ReturnMessage_m.h"

/**
 * TODO - Generated class
 */
class DemoBase : public OPP::cSimpleModule
{
        // Definitions
    private:
        using ReturnMsgPtr = oplkMessages::ReturnMessage*;
        using MessageFun = std::function<void(ReturnMsgPtr)>;
        using DispatcherCont = std::map<int, MessageFun>;

        // C-Tor
    public:
        DemoBase();

        // Methods
    protected:
        virtual void activity();
        virtual void initialize();
        virtual void handleMessage(OPP::cMessage *rawMsg);

    private:
        void initPowerlink();
        void initApp();

        void processApiReturn(ReturnMsgPtr msg);
        void processAppReturn(ReturnMsgPtr msg);
        void processStackShutdown();

        // Member
    private:
        DispatcherCont mDispatch;
        OPP::cGate* mApiCallGate;
        OPP::cGate* mAppCallGate;
};

#endif
