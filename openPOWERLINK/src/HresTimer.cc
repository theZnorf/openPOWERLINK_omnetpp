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

#include "HresTimer.h"
#include "MsgPtr.h"

Define_Module(HresTimer);

struct HresTimerInfo
{
        HresTimerHandle handle;
        simtime_t time;
        HresTimer::TimerCallback callback;
        HresTimer::ArgumentType argument;
        bool cont;
        HresTimer::TimerEventArgs eventArgs;
};

void HresTimer::initialize()
{
    // initialze dispatcher for this module
    interface::OplkHresTimer::getInstance().initHresTimer(this);
}

bool HresTimer::modifyTimer(HresTimerHandle* handle, TimeType timeNs, TimerCallback callback, ArgumentType arg,
        bool cont)
{
    // check handle
    if (handle == nullptr)
    {
        EV << "HresTimer::modifyTimer invalid handle" << std::endl;
    }

    // build info struct
    HresTimerInfo info;
    info.handle = getNewHandle();
    info.time = simtime_t(timeNs, SimTimeUnit::SIMTIME_NS);
    info.callback = callback;
    info.argument = arg;
    info.cont = cont;

    // configure timer
    if (!configureTimer(&info))
    {
        EV << "HresTimer::modifyTimer configuration failed" << std::endl;
        return false;
    }
    *handle = info.handle;
    return true;
}

bool HresTimer::deleteTimer(HresTimerHandle* handle)
{
    if (handle == nullptr)
    {
        EV << "HresTimer::deleteTimer invalid handle" << std::endl;
        return false;
    }

    return removeTimer(*handle);
}

void HresTimer::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        auto timerMsg = dynamic_cast<TimerMsg*>(msg.get());

        if (timerMsg != nullptr)
        {
            // get timer
            auto timer = getTimerInfo(timerMsg->getTimerHandle());
            if (timer != nullptr)
            {
                // create timer event arg
                TimerEventArgs arg;

                // call callback
                //timer->callback(arg);
            }
        }
    }
}
