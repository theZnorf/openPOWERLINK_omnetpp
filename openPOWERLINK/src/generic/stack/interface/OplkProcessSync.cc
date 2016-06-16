/*
 * OplkProcessSync.cc
 *
 *  Created on: Jun 15, 2016
 *      Author: franz
 */

#include <OplkProcessSync.h>
#include "OplkException.h"
#include "ProcessSync.h"

namespace interface
{

    OplkProcessSync::OplkProcessSync()
    {
    }

    OplkProcessSync::~OplkProcessSync()
    {
    }

    void OplkProcessSync::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        OPLK::tProcessSyncFunctions functions;
        functions.pfnCbProcessSync = OplkProcessSync::processSyncCb;

        // set function pointer of interface
        auto setFunctions = helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tProcessSyncFunctions>("sim_setProcessSyncFunctions");

        if (setFunctions == nullptr)
            throw std::runtime_error("OplkProcessSync::setFunctions - unable to resolve setFunctions function");

        auto ret = setFunctions(handle,functions);

        if (ret != TRUE)
            throw std::runtime_error("OplkProcessSync::setFunctions - unable to set function pointer");
    }

    OplkProcessSync& OplkProcessSync::getInstance()
    {
        static OplkProcessSync eventInstance;

        return eventInstance;
    }

    OplkProcessSync::ErrorType OplkProcessSync::processSyncCb(InstanceHandle handle)
    {
        try
        {
            OplkProcessSync::getInstance().getModule(handle)->processSyncCb();
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
