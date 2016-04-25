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

#ifndef __OPENPOWERLINK_HRESTIMER_H_
#define __OPENPOWERLINK_HRESTIMER_H_

#include <omnetpp.h>
#include <TimerBase.h>
#include <functional>

using HresTimerHandle = unsigned long;
struct HresTimerInfo;


class HresTimer : public TimerBase<HresTimerHandle, HresTimerInfo>
{
        // Definitions
    public:
        using TimeType = unsigned long long;
        using ArgumentType = unsigned long;
        using ErrorType = unsigned long;

        struct TimerEventArgs
        {
                HresTimerHandle handle;
                ArgumentType value;
        };

        using TimerCallback = std::function<ErrorType(TimerEventArgs*)>;


        // Methods
    public:
        bool modifyTimer(HresTimerHandle* handle, TimeType time, TimerCallback callback, ArgumentType arg, bool cont);
        bool deleteTimer(HresTimerHandle* handle);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

        // Member
    private:
        HresTimerHandle mNextHandle;
};

#endif
