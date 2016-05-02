/*
 * OplkApi.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKAPI_H_
#define OPLKAPI_H_

#include <functional>
#include "OplkBase.h"

namespace interface
{
    namespace api
    {
        using ErrorType = OPLK::ErrorType;
        using ApiInitParam = OPLK::tOplkApiInitParam;
        using NmtEvent = OPLK::tNmtEvent;
        using ObdCallbackParam = OPLK::tObdCbParam;
        using ObdSize = OPLK::tObdSize;
        using SdoComConHdl = OPLK::tSdoComConHdl;
        using SdoType = OPLK::tSdoType;
        using ObdAlConnHdl = OPLK::tObdAlConHdl;
        using BoolType = OPLK::BoolType;
        using AsndFrame = OPLK::tAsndFrame;
        using PlkFrame = OPLK::tPlkFrame;
        using IdentResponse = OPLK::tIdentResponse;
        using ApiStackInfo = OPLK::tOplkApiStackInfo;
        using SocTimeInfo = OPLK::tOplkApiSocTimeInfo;
        using NmtNodeCommand = OPLK::tNmtNodeCommand;
        using AsndFilter = OPLK::tOplkApiAsndFilter;
        using ApiEventType = OPLK::tOplkApiEventType;
        using ApiEventArg = OPLK::tOplkApiEventArg;

        struct ApiFunctions
        {
                std::function<ErrorType(void)> initialize;
                std::function<ErrorType(ApiInitParam*)> create;
                std::function<ErrorType(void)> destroy;
                std::function<ErrorType(void)> exit;
                std::function<ErrorType(NmtEvent)> execNmtCommand;
                std::function<ErrorType(ObdCallbackParam)> cbGenericObdAccess;
                std::function<ErrorType(UINT, void*, UINT*, ObdSize*, UINT)> linkObject;
                std::function<ErrorType(SdoComConHdl*, UINT, UINT, UINT, void*, UINT*, SdoType, void*)> readObject;
                std::function<ErrorType(SdoComConHdl*, UINT, UINT, UINT, void*, UINT, SdoType, void*)> writeObject;
                std::function<ErrorType(ObdAlConnHdl)> finishUserObdAccess;
                std::function<ErrorType(BoolType)> enableUserObdAccess;
                std::function<ErrorType(SdoComConHdl)> freeSdoChannel;
                std::function<ErrorType(SdoComConHdl, UINT32)> abortSdoChannel;
                std::function<ErrorType(UINT, UINT, void*, UINT*)> readLocalObject;
                std::function<ErrorType(UINT, UINT, void*, UINT)> writeLocalObject;
                std::function<ErrorType(UINT8, AsndFrame*, size_t)> sendAsndFrame;
                std::function<ErrorType(PlkFrame*, UINT)> sendEthFrame;
                std::function<ErrorType(UINT8, AsndFilter)> setAsndForward;
                std::function<ErrorType(BoolType)> setNonPlkForward;
                std::function<ErrorType(void*)> postUserEvent;
                std::function<ErrorType(UINT, NmtNodeCommand)> triggerMnStateChange;
                std::function<ErrorType(BYTE*, UINT)> setCdcBuffer;
                std::function<ErrorType(const char*)> setOdArchivePath;
                std::function<ErrorType(const char*)> setCdcFilename;
                std::function<ErrorType(void)> process;
                std::function<ErrorType(UINT, IdentResponse**)> getIdentResponse;
                std::function<ErrorType(UINT8*)> getEthMacAddr;
                std::function<BoolType(void)> checkKernelStack;
                std::function<ErrorType(ULONG)> waitSyncEvent;
                std::function<UINT32(void)> getVersion;
                std::function<char*(void)> getVersionString;
                std::function<UINT32(void)> getStackConfiguration;
                std::function<ErrorType(ApiStackInfo*)> getStackInfo;
                std::function<ErrorType(SocTimeInfo*)> getSocTime;
                std::function<char*(ErrorType)> getRetStr;
        };

    }

    class OplkApi : public OplkBase<api::ApiFunctions*>
    {
            // Definitions
        public:

            // C-Tor / D-Tor
        private:
            OplkApi();
        public:
            virtual ~OplkApi();

            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkApi & getInstance();
    };

} /* namespace interface */

#endif /* OPLKAPI_H_ */
