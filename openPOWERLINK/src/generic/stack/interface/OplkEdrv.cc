/*
 * OplkEdrv.cc
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include <OplkEdrv.h>
#include "OplkException.h"
#include "DirectEdrv.h"

namespace interface
{

    OplkEdrv::OplkEdrv()
    {
    }

    OplkEdrv::~OplkEdrv()
    {
    }

    void OplkEdrv::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        OPLK::tEdrvFunctions functions;
        functions.pfnInit = OplkEdrv::initEdrv;
        functions.pfnExit = OplkEdrv::exitEdrv;
        functions.pfnGetMacAddr = OplkEdrv::getMacAddr;
        functions.pfnSendTxBuffer = OplkEdrv::sendTxBuffer;
        functions.pfnAllocTxBuffer = OplkEdrv::allocTxBuffer;
        functions.pfnFreeTxBuffer = OplkEdrv::freeTxBuffer;
        functions.pfnChangeRxBufferFiler = OplkEdrv::changeRxFilter;
        functions.pfnSetMulticastMacAddr = OplkEdrv::setRxMulticastMacAddr;
        functions.pfnClearMulticastMacAddr = OplkEdrv::clearRxMulticastMacAddr;

        // set function pointer of interface
        auto setFunctions = helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tEdrvFunctions>("sim_setEdrvFunctions");

        auto ret = setFunctions(handle,functions);

        if (ret != TRUE)
            throw std::runtime_error("OplkEdrv::setFunctions - unable to set function pointer");
    }

    OplkEdrv& OplkEdrv::getInstance()
    {
        static OplkEdrv oplkEdrv;

        return oplkEdrv;
    }

    OplkEdrv::ErrorType OplkEdrv::initEdrv(InstanceHandle handle, EdrvInitParamType* initParam)
    {
        if (initParam == nullptr)
            return OPLK::kErrorApiInvalidParam;

        try
        {
            OplkEdrv::getInstance().getModule(handle)->initEdrv(initParam);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::ErrorType OplkEdrv::exitEdrv(InstanceHandle handle)
    {
        try
        {
            OplkEdrv::getInstance().getModule(handle)->exitEdrv();
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::MacType OplkEdrv::getMacAddr(InstanceHandle handle)
    {
        try
        {
            return OplkEdrv::getInstance().getModule(handle)->getMacAddr();
        }
        catch (std::exception const &)
        {
            return nullptr;
        }
    }

    OplkEdrv::ErrorType OplkEdrv::sendTxBuffer(InstanceHandle handle, TxBufferType* txBuffer)
    {
        if (txBuffer == nullptr)
            return OPLK::kErrorEdrvInvalidParam;

        try
        {
            OplkEdrv::getInstance().getModule(handle)->sendTxBuffer(txBuffer);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::ErrorType OplkEdrv::allocTxBuffer(InstanceHandle handle, TxBufferType* txBuffer)
    {
        if (txBuffer == nullptr)
            return OPLK::kErrorEdrvInvalidParam;

        try
        {
            OplkEdrv::getInstance().getModule(handle)->allocTxBuffer(txBuffer);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::ErrorType OplkEdrv::freeTxBuffer(InstanceHandle handle, TxBufferType* txBuffer)
    {
        if (txBuffer == nullptr)
            return OPLK::kErrorEdrvInvalidParam;

        try
        {
            OplkEdrv::getInstance().getModule(handle)->freeTxBuffer(txBuffer);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::ErrorType OplkEdrv::changeRxFilter(InstanceHandle handle, FilterType* filter, unsigned int count,
            unsigned int entryChanged, unsigned int changeFlags)
    {
        if (filter == nullptr)
            return OPLK::kErrorEdrvInvalidParam;

        try
        {
            OplkEdrv::getInstance().getModule(handle)->changeRxFilter(filter, count, entryChanged, changeFlags);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::ErrorType OplkEdrv::clearRxMulticastMacAddr(InstanceHandle handle, MacType macAddr)
    {
        try
        {
            OplkEdrv::getInstance().getModule(handle)->clearRxMulticastMacAddr(macAddr);
        }
        catch (OplkException const & e)
        {
            return e.errorNumber();
        }
        catch (std::exception const &)
        {
            return OPLK::kErrorGeneralError;
        }

        return OPLK::kErrorOk;
    }

    OplkEdrv::ErrorType OplkEdrv::setRxMulticastMacAddr(InstanceHandle handle, MacType macAddr)
    {
        try
        {
            OplkEdrv::getInstance().getModule(handle)->setRxMulticastMacAddr(macAddr);
        }
        catch (OplkException const & e)
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
