/*
 * OplkSdoUdp.cc
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include <OplkSdoUdp.h>
#include "OplkException.h"
#include "SdoUdp.h"

namespace interface
{

    OplkSdoUdp::OplkSdoUdp()
    {
    }

    OplkSdoUdp::~OplkSdoUdp()
    {
    }

    void OplkSdoUdp::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        // resolve exported interface functions in module
        auto interfaceFunctions = getModule(handle)->getInterfaceFunctions();
        interfaceFunctions->receiveDataCb = helper->getFunction<OPLK::ErrorType, ConnectionType*, AsySdoSeq*, unsigned int>("sim_sdoudpReceiveData");

        OPLK::tSdoUdpFunctions functions;
        functions.pfnCreateSocket = OplkSdoUdp::createSocket;
        functions.pfnCloseSocket = OplkSdoUdp::closeSocket;
        functions.pfnSendToSocket = OplkSdoUdp::sendToSocket;
        functions.pfnCiritcalSection = OplkSdoUdp::ciricalSection;

        // set function pointer of interface
        auto setFunctions = helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tSdoUdpFunctions>("sim_setSdoUdpFunctions");

        if (setFunctions == nullptr)
            throw std::runtime_error("OplkSdoUdp::setFunctions - unable to resolve setFunctions function");

        auto ret = setFunctions(handle,functions);

        if (ret != TRUE)
            throw std::runtime_error("OplkSdoUdp::setFunctions - unable to set function pointer");
    }

    OplkSdoUdp& OplkSdoUdp::getInstance()
    {
        static OplkSdoUdp instance;

        return instance;
    }

    OplkSdoUdp::ErrorType OplkSdoUdp::createSocket(InstanceHandle handle, ConnectionType* connection)
    {
        if (connection == nullptr)
            return OPLK::kErrorApiInvalidParam;

        try
        {
            OplkSdoUdp::getInstance().getModule(handle)->createSocket(connection);
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

    OplkSdoUdp::ErrorType OplkSdoUdp::closeSocket(InstanceHandle handle)
    {
        try
        {
            OplkSdoUdp::getInstance().getModule(handle)->closeSocket();
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

    OplkSdoUdp::ErrorType OplkSdoUdp::sendToSocket(InstanceHandle handle, ConnectionType* connection, FrameType* data,
            SizeType size)
    {
        if ((connection == nullptr) || (data == nullptr))
            return OPLK::kErrorApiInvalidParam;

        try
        {
            OplkSdoUdp::getInstance().getModule(handle)->sendToSocket(connection, data, size);
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

    void OplkSdoUdp::ciricalSection(InstanceHandle handle, OPLK::BoolType enable)
    {
        try
        {
            OplkSdoUdp::getInstance().getModule(handle)->ciricalSection(enable == TRUE);
        }
        catch (std::exception const &)
        {
            // TODO: handle exception
        }
    }

} /* namespace interface */
