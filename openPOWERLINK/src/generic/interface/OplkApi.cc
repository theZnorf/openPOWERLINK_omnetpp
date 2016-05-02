/*
 * OplkApi.cc
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include <OplkApi.h>

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
    // set functions to api
    auto functions = getModule(handle);

    functions->initialize = helper->getFunction < api::ErrorType > ("oplk_initialize");
    functions->create = helper->getFunction<api::ErrorType, api::ApiInitParam*>("oplk_create");
    functions->destroy = helper->getFunction < api::ErrorType > ("oplk_destroy");
    functions->exit = helper->getFunction < api::ErrorType > ("oplk_exit");
    functions->execNmtCommand = helper->getFunction<api::ErrorType, api::NmtEvent>("oplk_execNmtCommand");
    functions->cbGenericObdAccess = helper->getFunction<api::ErrorType, api::ObdCallbackParam>(
            "oplk_cbGenericObdAccess");
    functions->linkObject = helper->getFunction<api::ErrorType, UINT, void*, UINT*, api::ObdSize*, UINT>(
            "oplk_linkObject");
    functions->readObject = helper->getFunction<api::ErrorType, api::SdoComConHdl*, UINT, UINT, UINT, void*, UINT*,
            api::SdoType, void*>("oplk_readObject");
    functions->writeObject = helper->getFunction<api::ErrorType, api::SdoComConHdl*, UINT, UINT, UINT, void*, UINT,
            api::SdoType, void*>("oplk_writeObject");
    functions->finishUserObdAccess = helper->getFunction<api::ErrorType, api::ObdAlConnHdl>("oplk_finishUserObdAccess");
    functions->enableUserObdAccess = helper->getFunction<api::ErrorType, api::BoolType>("oplk_enableUserObdAccess");
    functions->freeSdoChannel = helper->getFunction<api::ErrorType, api::SdoComConHdl>("oplk_freeSdoChannel");
    functions->abortSdoChannel = helper->getFunction<api::ErrorType, api::SdoComConHdl, UINT32>("oplk_abortSdo");
    functions->readLocalObject = helper->getFunction<api::ErrorType, UINT, UINT, void*, UINT*>("oplk_readLocalObject");
    functions->writeLocalObject = helper->getFunction<api::ErrorType, UINT, UINT, void*, UINT>("oplk_writeLocalObject");
    functions->sendAsndFrame = helper->getFunction<api::ErrorType, UINT8, api::AsndFrame*, size_t>(
            "oplk_sendAsndFrame");
    functions->sendEthFrame = helper->getFunction<api::ErrorType, api::PlkFrame*, UINT>("oplk_sendEthFrame");
    functions->setAsndForward = helper->getFunction<api::ErrorType, UINT8, api::AsndFilter>("oplk_postUserEvent");
    functions->setNonPlkForward = helper->getFunction<api::ErrorType, api::BoolType>("oplk_setNonPlkForward");
    functions->postUserEvent = helper->getFunction<api::ErrorType, void*>("oplk_postUserEvent");
    functions->triggerMnStateChange = helper->getFunction<api::ErrorType, UINT, api::NmtNodeCommand>(
            "oplk_triggerMnStateChange");
    functions->setCdcBuffer = helper->getFunction<api::ErrorType, BYTE*, UINT >("oplk_setCdcBuffer");
    functions->setCdcFilename = helper->getFunction<api::ErrorType, const char*>("oplk_setCdcFilename");
    functions->setOdArchivePath = helper->getFunction<api::ErrorType, const char*>("oplk_setOdArchivePath");
    functions->process = helper->getFunction < api::ErrorType > ("oplk_process");
    functions->getIdentResponse = helper->getFunction<api::ErrorType, UINT, api::IdentResponse**>(
            "oplk_getIdentResponse");
    functions->getEthMacAddr = helper->getFunction<api::ErrorType, UINT8*>("oplk_getEthMacAddr");
    functions->checkKernelStack = helper->getFunction < api::BoolType > ("oplk_checkKernelStack");
    functions->waitSyncEvent = helper->getFunction<api::ErrorType, ULONG>("oplk_waitSyncEvent");
    functions->getVersion = helper->getFunction<UINT32>("oplk_getVersion");
    functions->getVersionString = helper->getFunction<char*>("oplk_getVersionString");
    functions->getStackConfiguration = helper->getFunction<UINT32>("oplk_getStackConfiguration");
    functions->getStackInfo = helper->getFunction<api::ErrorType, api::ApiStackInfo*>("oplk_getStackInfo");
    functions->getSocTime = helper->getFunction<api::ErrorType, api::SocTimeInfo*>("oplk_getSocTime");
    functions->getRetStr = helper->getFunction<char*, api::ErrorType>("debugstr_getRetValStr");
}
