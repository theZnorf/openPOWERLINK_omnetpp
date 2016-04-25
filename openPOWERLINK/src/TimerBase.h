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

#ifndef TIMERBASE_H_
#define TIMERBASE_H_

#include <omnetpp.h>
#include <map>

#include "TimerMsg_m.h"

template<typename THandle, typename TInfo>
class TimerBase : public cSimpleModule
{
        // Definitions
    protected:
        using TimerCont = std::map<THandle, TInfo>;

        // Methods
    protected:
        void scheduleTimer(TInfo* info)
        {
            scheduleAt(simTime() + info->time, createTimerMessage(info->handle));
        }

        void scheduleTimer(TInfo info)
        {
            scheduleTimer(&info);
        }

        THandle getNewHandle()
        {
            return mNextHandle++;
        }

        bool configureTimer(TInfo* info)
        {
            // create time info structure in map, or modify if alread in use
            auto ret = mTimers.emplace(info->handle, *info);

            // schedule timer
            scheduleTimer(std::get < 1 > (*std::get < 0 > (ret)));

            return true;
        }

        bool configureTimer(TInfo info)
        {
            return configureTimer(&info);
        }

        TInfo* getTimerInfo(THandle handle)
        {
            // get according timer info
            auto iter = mTimers.find(handle);

            return (iter != mTimers.end()) ? &std::get<1>(*iter) : nullptr;
        }

        bool removeTimer(THandle handle)
        {
            auto ret = mTimers.erase(handle);
            return ret == 1;
        }

    private:
        cMessage* createTimerMessage(THandle handle)
        {
            auto msg = new TimerMsg();
            msg->setTimerHandle(handle);

            return msg;
        }

        // Member
    protected:
        TimerCont mTimers;
    private:
        THandle mNextHandle;
};

#endif /* TIMERBASE_H_ */
