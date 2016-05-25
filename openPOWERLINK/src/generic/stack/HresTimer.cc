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
        bool expired;
};

void HresTimer::initialize()
{
    try
    {
        // resolve library info parameter
        std::string libName = par("libName");
        interface::OplkHresTimer::Instance numberOfInstances = par("numberOfInstances");

        // init stub
        interface::OplkHresTimer::setLibraryInfo(libName, numberOfInstances);
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

    HresTimerInfo info;
    // find timer
    auto infoPtr = this->getTimerInfo(*handle);

    // check if timer already exists
    if (infoPtr == nullptr)
    {
        // create new handle
        info.handle = getNewHandle();
        infoPtr = &info;
    }

    // fill info
    infoPtr->time = simtime_t(timeNs, SimTimeUnit::SIMTIME_NS);
    infoPtr->callback = callback;
    infoPtr->argument = arg;
    infoPtr->cont = cont;
    infoPtr->expired = false;

    // configure timer
    if (!configureTimer(infoPtr))
    {
        EV << "HresTimer::modifyTimer configuration failed" << std::endl;
        throw interface::OplkException("timer not valid", OPLK::kErrorTimerNoTimerCreated);
    }

    *handle = infoPtr->handle;

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

    removeTimer(*handle);

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
                timer->expired = true;

                // create timer event arg
                TimerEventArgs arg;
                arg.timerHdl.handle = timer->handle;
                arg.argument.value = timer->argument;

                // call callback
                timer->callback(&arg);

                // check if continous timer
                if (timer->cont)
                {
                    // reschedule timer
                    scheduleTimer(timer);
                }
                else if (timer->expired) // remove timer info when not rescheduled by callback
                {
                    removeTimer(timer->handle);

                    refreshDisplay();
                }
            }
        }
    }
}

void HresTimer::refreshDisplay()
{
    getDisplayString().setTagArg("t",0,("Timers modified: " + std::to_string(mTimers.size())).c_str());
}
