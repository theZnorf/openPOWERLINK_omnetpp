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

#ifndef __OPENPOWERLINK_APPBASE_H_
#define __OPENPOWERLINK_APPBASE_H_

#include <omnetpp.h>
#include "UseApiBase.h"
#include "ReturnHandler.h"

class AppBase : public UseApiBase
{
        // Definitions
    public:
        enum class AppCallType
            : short
            {
                init, shutdown, processSync
        };

        // C-Tor
    public:
            AppBase();

        // Methods
    protected:
        virtual void initialize();
        virtual void handleOtherMessage(MessagePtr msg);

        virtual interface::api::ErrorType initApp();
        virtual interface::api::ErrorType processSync();
        virtual void shutdownApp();

        // Member
    private:
        OPP::cGate* mReturnGate;

    protected:
        HandlerPtr mRet;
};

#endif
