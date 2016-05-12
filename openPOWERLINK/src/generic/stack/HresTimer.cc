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

#include <string>
#include "HresTimer.h"
#include "MsgPtr.h"
#include "interface/OplkException.h"

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
    try
    {
        // initialze dispatcher for this module
        interface::OplkHresTimer::getInstance().initModule(this);
    }
    catch (std::exception const & e)
    {
        EV << "HresTimer -  init : " << e.what() << std::endl;
        throw e;
    }
}


void HresTimer::initTimer()
{
    bubble("HresTimer initialized");
    getDisplayString().setTagArg("i",1,"green");
    refreshDisplay();
}

void HresTimer::exitTimer()
{
    bubble("HresTimer exited");
    getDisplayString().setTagArg("i",1,"red");
    getDisplayString().setTagArg("t",0,"");
}

void HresTimer::modifyTimer(HresTimerHandle* handle, TimeType timeNs, TimerCallback callback, ArgumentType arg,
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
        throw interface::OplkException("timer not valid", OPLK::kErrorTimerNoTimerCreated);
    }
    *handle = info.handle;

    bubble("Timer modified");

    refreshDisplay();
}

void HresTimer::deleteTimer(HresTimerHandle* handle)
{
    if (handle == nullptr)
    {
        EV << "HresTimer::deleteTimer invalid handle" << std::endl;
        throw interface::OplkException("invalid handle", OPLK::kErrorTimerInvalidHandle);
    }

    if (!removeTimer(*handle))
        throw interface::OplkException("timer not valid", OPLK::kErrorTimerNoTimerCreated);

    refreshDisplay();
}

void HresTimer::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        auto timerMsg = dynamic_cast<TimerMsg*>(msg.get());

        if (timerMsg != nullptr)
        {

            bubble("Timer expired");

            // get timer
            auto timer = getTimerInfo(timerMsg->getTimerHandle());
            if (timer != nullptr)
            {
                // create timer event arg
                TimerEventArgs arg;

                // call callback
                timer->callback(&arg);
            }
        }
    }
}

void HresTimer::refreshDisplay()
{
    getDisplayString().setTagArg("t",0,("Timers modified: " + std::to_string(mTimers.size())).c_str());
}
