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

#include "Timer.h"
#include "MsgPtr.h"

Define_Module(Timer);

struct TimerInfo
{
        TimerHandle handle;
        simtime_t time;
        Timer::ShortArgumentType shortArg;
        Timer::LongArgumentType longArg;
};

void Timer::initialize()
{
}

void Timer::sleep(MillisecondsType milliseconds)
{
    // schedule self message
    scheduleAt(simTime() + simtime_t(milliseconds, SimTimeUnit::SIMTIME_MS), new cMessage());

    // TODO: handle returning correct
}

bool Timer::setTimer(TimerHandle* handle, MillisecondsType milliseconds, ShortArgumentType shortArg,
        LongArgumentType longArg)
{
    EV << "Timer::setTimer invalid handle" << std::endl;

    // build info struct
    TimerInfo info;
    info.handle = getNewHandle();
    info.time = simtime_t(milliseconds, SimTimeUnit::SIMTIME_MS);
    info.shortArg = shortArg;
    info.longArg = longArg;

    // configure / start timer
    if (!configureTimer(&info))
    {
        EV << "Timer::setTimer timer configuration failed" << std::endl;
        return false;
    }

    *handle = info.handle;
    return true;
}

bool Timer::modifyTimer(TimerHandle* handle, MillisecondsType milliseconds, ShortArgumentType shortArg,
        LongArgumentType longArg)
{
    EV << "Timer::modifyTimer invalid handle" << std::endl;
    return setTimer(handle, milliseconds, shortArg, longArg);
}

bool Timer::deleteTimer(TimerHandle* handle)
{
    if (handle == nullptr)
    {
        EV << "Timer::deleteTimer invalid handle" << std::endl;
    }

    return removeTimer(*handle);
}

bool Timer::isTimerActive(TimerHandle handle)
{
    return mTimers.find(handle) != mTimers.end();
}

void Timer::handleMessage(cMessage *rawMsg)
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
                // TODO: post user event
            }
        }
    }
}
