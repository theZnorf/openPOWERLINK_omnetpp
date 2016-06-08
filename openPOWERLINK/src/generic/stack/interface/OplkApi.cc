/*
 * OplkApi.cc
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include "OplkApi.h"
#include "Api.h"

namespace interface
{

    OplkApi::OplkApi()
    {
        // TODO Auto-generated constructor stub

    }

    OplkApi::~OplkApi()
    {
        // TODO Auto-generated destructor stub
    }

    OplkApi& OplkApi::getInstance()
    {
        static OplkApi instance;
        return instance;
    }

} /* namespace interface */

void interface::OplkApi::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
{
    using namespace api;

    // get setFunctions function
    OPLK::tApiFunctions callbacks;
    callbacks.pfnCbProcessSync = &OplkApi::processSyncCb;
    callbacks.pfnCbEvent = &OplkApi::eventCb;

    auto setFunctions = helper->getFunction<OPLK::BoolType, InstanceHandle, OPLK::tApiFunctions>("sim_setApiFunctions");

    if (setFunctions == nullptr)
        throw std::runtime_error("OplkApi::setFunctions - unable to resolve setFunctions function");

    auto ret = setFunctions(handle, callbacks);

    if (ret != TRUE)
        throw std::runtime_error("OplkEdrv::setFunctions - unable to set function pointer");

    // set functions to api
    auto functions = getModule(handle)->getApiFunctions();

    // generic api functions
    functions->initialize = helper->getFunction < ErrorType > ("oplk_initialize");
    functions->create = helper->getFunction<ErrorType, ApiInitParam*>("sim_oplkCreate");
    functions->destroy = helper->getFunction < ErrorType > ("oplk_destroy");
    functions->exit = helper->getFunction<void>("oplk_exit");
    functions->execNmtCommand = helper->getFunction<ErrorType, NmtEvent>("oplk_execNmtCommand");
    functions->cbGenericObdAccess = helper->getFunction<ErrorType, ObdCallbackParam*>("oplk_cbGenericObdAccess");
    functions->linkObject = helper->getFunction<ErrorType, UINT, void*, UINT*, ObdSize*, UINT>("oplk_linkObject");
    functions->readObject =
            helper->getFunction<ErrorType, SdoComConHdl*, UINT, UINT, UINT, void*, UINT*, SdoType, void*>(
                    "oplk_readObject");
    functions->writeObject =
            helper->getFunction<ErrorType, SdoComConHdl*, UINT, UINT, UINT, void*, UINT, SdoType, void*>(
                    "oplk_writeObject");
    functions->finishUserObdAccess = helper->getFunction<ErrorType, ObdAlConnHdl*>("oplk_finishUserObdAccess");
    functions->enableUserObdAccess = helper->getFunction<ErrorType, BoolType>("oplk_enableUserObdAccess");
    functions->freeSdoChannel = helper->getFunction<ErrorType, SdoComConHdl>("oplk_freeSdoChannel");
    functions->abortSdoChannel = helper->getFunction<ErrorType, SdoComConHdl, UINT32>("oplk_abortSdo");
    functions->readLocalObject = helper->getFunction<ErrorType, UINT, UINT, void*, UINT*>("oplk_readLocalObject");
    functions->writeLocalObject = helper->getFunction<ErrorType, UINT, UINT, void*, UINT>("oplk_writeLocalObject");
    functions->sendAsndFrame = helper->getFunction<ErrorType, UINT8, AsndFrame*, size_t>("oplk_sendAsndFrame");
    functions->sendEthFrame = helper->getFunction<ErrorType, PlkFrame*, UINT>("oplk_sendEthFrame");
    functions->setAsndForward = helper->getFunction<ErrorType, UINT8, AsndFilter>("oplk_postUserEvent");
    functions->setNonPlkForward = helper->getFunction<ErrorType, BoolType>("oplk_setNonPlkForward");
    functions->postUserEvent = helper->getFunction<ErrorType, void*>("oplk_postUserEvent");
    functions->triggerMnStateChange = helper->getFunction<ErrorType, UINT, NmtNodeCommand>("oplk_triggerMnStateChange");
    functions->setCdcBuffer = helper->getFunction<ErrorType, BYTE*, UINT >("oplk_setCdcBuffer");
    functions->setCdcFilename = helper->getFunction<ErrorType, const char*>("oplk_setCdcFilename");
    functions->setOdArchivePath = helper->getFunction<ErrorType, const char*>("oplk_setOdArchivePath");
    functions->process = helper->getFunction < ErrorType > ("oplk_process");
    functions->getIdentResponse = helper->getFunction<ErrorType, UINT, IdentResponse**>("oplk_getIdentResponse");
    functions->getEthMacAddr = helper->getFunction<ErrorType, UINT8*>("oplk_getEthMacAddr");
    functions->checkKernelStack = helper->getFunction < BoolType > ("oplk_checkKernelStack");
    functions->waitSyncEvent = helper->getFunction<ErrorType, ULONG>("oplk_waitSyncEvent");
    functions->getVersion = helper->getFunction<UINT32>("oplk_getVersion");
    functions->getVersionString = helper->getFunction<char*>("oplk_getVersionString");
    functions->getStackConfiguration = helper->getFunction<UINT32>("oplk_getStackConfiguration");
    functions->getStackInfo = helper->getFunction<ErrorType, ApiStackInfo*>("oplk_getStackInfo");
    functions->getSocTime = helper->getFunction<ErrorType, SocTimeInfo*>("oplk_getSocTime");

    // process image functions
    functions->allocProcessImage = helper->getFunction<ErrorType, UINT, UINT>("oplk_allocProcessImage");
    functions->freeProcessImage = helper->getFunction < ErrorType > ("oplk_freeProcessImage");
    functions->linkProcessImageObject = helper->getFunction<ErrorType, UINT, UINT, UINT, BoolType, ObdSize, UINT*>(
            "oplk_linkProcessImageObject");
    functions->exchangeProcessImageIn = helper->getFunction < ErrorType > ("oplk_exchangeProcessImageIn");
    functions->exchangeProcessImageOut = helper->getFunction < ErrorType > ("oplk_exchangeProcessImageOut");
    functions->getProcessImageIn = helper->getFunction<void*>("oplk_getProcessImageIn");
    functions->getProcessImageOut = helper->getFunction<void*>("oplk_getProcessImageOut");

    functions->setupProcessImage = helper->getFunction < ErrorType > ("oplk_setupProcessImage");

    functions->triggerPresForward = helper->getFunction<ErrorType, UINT >("oplk_triggerPresForward");
}

interface::OplkApi::ErrorType interface::OplkApi::processSyncCb(InstanceHandle handle)
{
    try
    {
        OplkApi::getInstance().getModule(handle)->processSyncCb();
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

interface::OplkApi::ErrorType interface::OplkApi::eventCb(InstanceHandle handle, ApiEventType eventType, ApiEventArg* eventArg,
        void* userArg)
{
    try
    {
        OplkApi::getInstance().getModule(handle)->eventCb(eventType, eventArg, userArg);
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
