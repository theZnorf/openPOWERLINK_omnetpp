/*
 * OplkApiEvent.cc
 *
 *  Created on: Jun 15, 2016
 *      Author: franz
 */

#include <OplkApiEvent.h>
#include "OplkException.h"
#include "ApiEvent.h"

namespace interface
{

    OplkApiEvent::OplkApiEvent()
    {
    }

    OplkApiEvent::~OplkApiEvent()
    {
    }

    void OplkApiEvent::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        OPLK::tApiEventFunctions functions;
        functions.pfnCbEvent = OplkApiEvent::eventCb;

        // set function pointer of interface
        auto setFunctions = helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tApiEventFunctions>("sim_setApiEventFunctions");

        if (setFunctions == nullptr)
            throw std::runtime_error("OplkApiEvent::setFunctions - unable to resolve setFunctions function");

        auto ret = setFunctions(handle,functions);

        if (ret != TRUE)
            throw std::runtime_error("OplkApiEvent::setFunctions - unable to set function pointer");
    }

    OplkApiEvent& OplkApiEvent::getInstance()
    {
        static OplkApiEvent eventInstance;

        return eventInstance;
    }

    OplkApiEvent::ErrorType OplkApiEvent::eventCb(InstanceHandle handle, ApiEventType eventType, ApiEventArg* eventArg,
            void* userArg)
    {
        try
        {
            OplkApiEvent::getInstance().getModule(handle)->eventCb(eventType, eventArg, userArg);
        }
        catch (interface::OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

} /* namespace interface */
