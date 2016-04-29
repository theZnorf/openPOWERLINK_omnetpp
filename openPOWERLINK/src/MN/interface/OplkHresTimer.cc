/*
 * OplkHresTimer.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#include "OplkHresTimer.h"

#include "HresTimer.h"
#include "OplkException.h"

using namespace std;
using namespace interface;

OplkHresTimer::OplkHresTimer()
{
    // TODO Auto-generated constructor stub

}

OplkHresTimer::~OplkHresTimer()
{
    // TODO Auto-generated destructor stub
}

OplkHresTimer& OplkHresTimer::getInstance()
{
    static OplkHresTimer timer;

    return timer;
}

OPLK::ErrorType OplkHresTimer::modifyTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle, TimeType time, TimerCallback callback,
        ArgumentType arg, OPLK::BoolType cont)
{
    // check parameter
    if (timerHandle == nullptr)
        return OPLK::kErrorTimerInvalidHandle;

    try
    {
        HresTimer::TimerCallback cb(callback);

        // call module method
        OplkHresTimer::getInstance().getModule(instanceHandle)->modifyTimer(timerHandle, time, cb, arg, cont == TRUE);
    }
    catch (OplkException & e)
    {
        return e.errorNumber();
    }
    catch (exception & e)
    {
        return OPLK::kErrorGeneralError;
    }

    return OPLK::kErrorOk;
}

OplkHresTimer::ErrorType OplkHresTimer::deleteTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle)
{
    // check parameter
    if (timerHandle == nullptr)
        return OPLK::kErrorTimerInvalidHandle;

    try
    {
        // call module method
        OplkHresTimer::getInstance().getModule(instanceHandle)->deleteTimer(timerHandle);
    }
    catch (OplkException & e)
    {
        return e.errorNumber();
    }
    catch (exception & e)
    {
        return OPLK::kErrorGeneralError;
    }

    return OPLK::kErrorOk;
}

void OplkHresTimer::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
{
    OPLK::tHresTimerFunctions functions;
    functions.pfnModifyHresTimer = OplkHresTimer::modifyTimer;
    functions.pfnDeleteHresTimer = OplkHresTimer::deleteTimer;

    // set function pointer of interface
    auto setFunctions = helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tHresTimerFunctions>("sim_setHresTimerFunctions");

    auto ret = setFunctions(handle, functions);

    if (ret != TRUE)
        throw runtime_error("OplkHresTimer::setFunctions - unable to set function pointer");
}
