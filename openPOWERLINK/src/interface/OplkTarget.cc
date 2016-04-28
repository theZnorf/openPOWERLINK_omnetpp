/*
 * OplkTarget.cc
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include <OplkTarget.h>
#include "Target.h"
#include "OplkException.h"

namespace interface
{

    OplkTarget::OplkTarget()
    {
        // TODO Auto-generated constructor stub

    }

    OplkTarget::~OplkTarget()
    {
        // TODO Auto-generated destructor stub
    }

    void OplkTarget::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        OPLK::tTargetFunctions functions;
        functions.pfnMsleep = OplkTarget::msleep;
        functions.pfnSetIp = OplkTarget::setIpAddress;
        functions.pfnSetDefaultGateway = OplkTarget::setDefaultGateway;
        functions.pfnGetTick = OplkTarget::getTickCount;
        functions.pfnSetLed = OplkTarget::setLed;

        // set function pointer of interface
        if (helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tTargetFunctions>("sim_setTargetFunctions")(handle, functions) != TRUE)
            throw std::runtime_error("OplkHresTimer::setFunctions - unable to set function pointer");
    }

    OplkTarget& OplkTarget::getInstance()
    {
        static OplkTarget target;

        return target;
    }

    void OplkTarget::msleep(InstanceHandle instanceHandle, MilliSecondsType milliseconds)
    {
        try
        {
            OplkTarget::getInstance().getModule(instanceHandle)->msleep(milliseconds);
        }
        catch (std::exception const &)
        {
            // TODO: handle exception
        }
    }

    OplkTarget::ErrorType OplkTarget::setIpAddress(InstanceHandle instanceHandle, char* interfaceName, AddressType ipAddress,
            AddressType subnetMask, MtuType mtu)
    {
        if (interfaceName == nullptr)
            return OPLK::kErrorApiInvalidParam;

        try
        {
            OplkTarget::getInstance().getModule(instanceHandle)->setIpAddress(interfaceName, ipAddress, subnetMask, mtu);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const & e)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkTarget::ErrorType OplkTarget::setDefaultGateway(InstanceHandle instanceHandle, AddressType defaultGateway)
    {
        try
        {
            OplkTarget::getInstance().getModule(instanceHandle)->setDefaultGateway(defaultGateway);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const & e)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkTarget::TickType OplkTarget::getTickCount(InstanceHandle instanceHandle)
    {
        try
        {
            return OplkTarget::getInstance().getModule(instanceHandle)->getTickCount();
        }
        catch (std::exception const &)
        {
            return 0;
        }
    }

    OplkTarget::ErrorType OplkTarget::setLed(InstanceHandle instanceHandle, LedType ledType, OPLK::BoolType ledOn)
    {
        try
        {
            OplkTarget::getInstance().getModule(instanceHandle)->setLed(ledType, ledOn == TRUE);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const & e)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

} /* namespace interface */
