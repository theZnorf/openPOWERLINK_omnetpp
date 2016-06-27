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
#include <string>

#include "TimerMsg_m.h"

template<typename THandle, typename TInfo>
class TimerBase : public OPP::cSimpleModule
{
        // Definitions
    protected:
        using TimerCont = std::map<THandle, TInfo>;

    protected:
//        TimerBase()
//            : OPP::cSimpleModule(16000)
//        { }

        // Methods
    protected:
//        void activity()
//        {
//            while(mRunning)
//            {
//                // receive message
//                auto msg = receive();
//
//                // handle message
//                handleRawMessage(msg);
//            }
//        }

        void scheduleTimer(TInfo* info)
        {
            auto msg = createTimerMessage(info->handle);

            msg->setName(("Timer Message: " + std::to_string(info->time.inUnit(SimTimeUnit::SIMTIME_US)) + " us").c_str());

            // cancel and delete previous scheduled message, or do nothing when nullptr is stored
            cancelAndDelete(info->scheduledMsg);

            // save pointer to scheduled timer message
            info->scheduledMsg = msg;

            scheduleAt(simTime() + info->time, msg);
        }

        THandle getNewHandle()
        {
            return mNextHandle++;
        }

        bool configureTimer(TInfo* info)
        {
            // create time info structure in map, or modify if alread in use
            mTimers.emplace(info->handle, *info);

            // schedule timer
            scheduleTimer(&mTimers.at(info->handle));

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
            auto info = getTimerInfo(handle);
            if ((info != nullptr) && (info->scheduledMsg != nullptr) && (info->scheduledMsg->isSelfMessage()))
                cancelAndDelete(info->scheduledMsg);

            auto ret = mTimers.erase(handle);
            return ret == 1;
        }

        //virtual void handleRawMessage(OPP::cMessage* msg) = 0;

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
        THandle mNextHandle = 1234;
        bool mRunning = false;
};

#endif /* TIMERBASE_H_ */
