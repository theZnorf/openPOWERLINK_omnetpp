/*
 * OplkTrace.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include <OplkTrace.h>
#include "Trace.h"

namespace interface
{

    OplkTrace::OplkTrace()
    {
        // TODO Auto-generated constructor stub

    }

    OplkTrace::~OplkTrace()
    {
        // TODO Auto-generated destructor stub
    }

    void OplkTrace::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        OPLK::tTraceFunctions functions;
        functions.pfnTrace = OplkTrace::trace;

        // set function pointer of interface
        if (helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tTraceFunctions>("sim_setTraceFunctions")(handle, functions) != TRUE)
            throw std::runtime_error("OplkHresTimer::setFunctions - unable to set function pointer");
    }

    OplkTrace OplkTrace::getInstance()
    {
        static OplkTrace traceInstance;

        return traceInstance;
    }

    void OplkTrace::trace(InstanceHandle handle, const char* msg)
    {
        try
        {
            OplkTrace::getInstance().getModule(handle)->trace(msg);
        }
        catch (std::exception const &)
        {
            //TODO: handle exception
        }
    }

} /* namespace interface */
