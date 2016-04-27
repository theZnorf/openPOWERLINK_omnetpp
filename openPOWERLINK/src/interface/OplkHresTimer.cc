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

void OplkHresTimer::initHresTimer(HresTimerPtr hresTimer)
{
    if (hresTimer == nullptr)
        throw invalid_argument("OplkHresTimer::initHresTimer - invalid hresTimer");

    // create info object with correct helper instance
    SharedLibraryHelper::HelperPtr helper;

    if (mHresTimerInfos.empty())
        helper = SharedLibraryHelper::createInstance(/*cLibName*/ "lib", /*cNumberOfInstances*/ 1);
    else
        helper = mHresTimerInfos.back().helper->getNextLibrary();

    HresTimerInfo info =
    {
            hresTimer,
            move(helper)
    };

    mHresTimerInfos.push_back(move(info));

    OPLK::tHresTimerFunctions functions;
    functions.pfnModifyHresTimer = OplkHresTimer::modifyTimer;
    functions.pfnDeleteHresTimer = OplkHresTimer::deleteTimer;


    // set function pointer of interface
    if (helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tHresTimerFunctions>("sim_setHresTimerFunctions")(mHresTimerInfos.size() -1, functions) != TRUE)
        throw runtime_error("OplkHresTimer::initHresTimer - unable to set function pointer");
}

OplkHresTimer::HresTimerPtr OplkHresTimer::getHresTimer(InstanceHandle handle) const
{
    if (handle < mHresTimerInfos.size())
        return mHresTimerInfos[handle].hresTimer;
    else
        throw out_of_range("OplkHresTimer::getHresTimer - invalid instance handle");
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
        OplkHresTimer::getInstance().getHresTimer(instanceHandle)->modifyTimer(timerHandle, time, cb, arg, cont == TRUE);
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

OPLK::ErrorType OplkHresTimer::deleteTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle)
{
    // check parameter
    if (timerHandle == nullptr)
        return OPLK::kErrorTimerInvalidHandle;

    try
    {
        // call module method
        OplkHresTimer::getInstance().getHresTimer(instanceHandle)->deleteTimer(timerHandle);
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
