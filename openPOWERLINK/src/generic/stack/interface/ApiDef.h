/*
 * ApiDef.h
 *
 *  Created on: May 3, 2016
 *      Author: franz
 */

#ifndef APIDEF_H_
#define APIDEF_H_

#include <omnetpp.h>
#include <functional>
#include "oplkinc.h"

namespace interface
{
    namespace api
    {
        using ErrorType = OPLK::ErrorType;
        using Error = OPLK::eOplkError;
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
        using NmtNodeCommandE = OPLK::eNmtNodeCommand;
        using AsndFilter = OPLK::tOplkApiAsndFilter;
        using ApiEventType = OPLK::tOplkApiEventType;
        using ApiEvent = OPLK::eOplkApiEventType;
        using ApiEventArg = OPLK::tOplkApiEventArg;
        using EventNmtStateChange = OPLK::tEventNmtStateChange;
        using EventError = OPLK::tEventError;
        using SdoComFinished = OPLK::tSdoComFinished;
        using ObdCbParam = OPLK::tObdCbParam;
        using ApiEventNode = OPLK::tOplkApiEventNode;
        using ApiEventBoot = OPLK::tOplkApiEventBoot;
        using CfmEventCnProgress = OPLK::tCfmEventCnProgress;
        using ApiEventCfmResult = OPLK::tOplkApiEventCfmResult;
        using ErrHistoryEntry = OPLK::tErrHistoryEntry;
        using ApiEventRcvAsnd = OPLK::tOplkApiEventRcvAsnd;
        using ApiEventPdoChange = OPLK::tOplkApiEventPdoChange;
        using ApiEventReceivedPres = OPLK::tOplkApiEventReceivedPres;
        using ApiEventReceivedNonPlk = OPLK::tOplkApiEventReceivedNonPlk;
        using ApiEventDefaultGwChange = OPLK::tOplkApiEventDefaultGwChange;
        using ApiEventReceivedSdoCom = OPLK::tOplkApiEventReceivedSdoCom;
        using ApiEventReceivedSdoSeq = OPLK::tOplkApiEventReceivedSdoSeq;
        using ApiEventUserObdAccess = OPLK::tOplkApiEventUserObdAccess;
        using HwParam = OPLK::tHwParam;
        using ObdInitParam = OPLK::tObdInitParam;
        using ObdEntry = OPLK::tObdEntry;
        using ObdSubEntry = OPLK::tObdSubEntry;
        using MsgType = OPLK::eMsgType;
        using NetTime = OPLK::tNetTime;
        using AsySdoCom = OPLK::tAsySdoCom;
        using NmtEventType = OPLK::eNmtEvent;
        using LedType = OPLK::eLedType;
        using EventType = OPLK::tEventType;
        using EventSource = OPLK::tEventSource;
        using EventSink = OPLK::tEventSink;
        using NmtState = OPLK::tNmtState;
        using NmtStateE = OPLK::eNmtState;
        using NmtNodeEvent = OPLK::tNmtNodeEvent;
        using NmtBootEvent = OPLK::tNmtBootEvent;
        using SdoComConState = OPLK::tSdoComConState;

        struct ApiFunctions
        {
                // generic api functions
                std::function<ErrorType(void)> initialize;
                std::function<ErrorType(ApiInitParam*)> create;
                std::function<ErrorType(void)> destroy;
                std::function<void(void)> exit;
                std::function<ErrorType(NmtEvent)> execNmtCommand;
                std::function<ErrorType(ObdCallbackParam*)> cbGenericObdAccess;
                std::function<ErrorType(UINT, void*, UINT*, ObdSize*, UINT)> linkObject;
                std::function<ErrorType(SdoComConHdl*, UINT, UINT, UINT, void*, UINT*, SdoType, void*)> readObject;
                std::function<ErrorType(SdoComConHdl*, UINT, UINT, UINT, void*, UINT, SdoType, void*)> writeObject;
                std::function<ErrorType(ObdAlConnHdl*)> finishUserObdAccess;
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

                // process image functions
                std::function<ErrorType(UINT, UINT)> allocProcessImage;
                std::function<ErrorType(void)> freeProcessImage;
                std::function<ErrorType(UINT, UINT, UINT, BoolType, ObdSize, UINT*)> linkProcessImageObject;
                std::function<ErrorType(void)> exchangeProcessImageIn;
                std::function<ErrorType(void)> exchangeProcessImageOut;
                std::function<void*(void)> getProcessImageIn;
                std::function<void*(void)> getProcessImageOut;

                std::function<ErrorType(void)> setupProcessImage;

                std::function<ErrorType(UINT)> triggerPresForward;
        };
    }
}

#endif /* APIDEF_H_ */
