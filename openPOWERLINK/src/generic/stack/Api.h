//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __OPENPOWERLINK_API_H_
#define __OPENPOWERLINK_API_H_

#include <omnetpp.h>
#include "interface/OplkApi.h"
#include "interface/ApiDef.h"
#include "MessageDispatcher.h"

class Api : public OPP::cSimpleModule
{
        // Definitions
    public:
        enum class ApiCallType : short
        {
            undefined = 0,
            init,
            create,
            destroy,
            exit,
            execNmtCommand,
            cbGenericObdAccess,
            linkObject,
            readObject,
            writeObject,

            finishUserObdAccess,
            enableUserObdAccess,
            freeSdoChannel,
            abortSdoChannel,
            readLocalObject,
            writeLocalObject,
            sendAsndFrame,
            sendEthFrame,
            setAsndForward,
            setNonPlkForward,
            postUserEvent,
            triggerMnStateChange,
            setCdcBuffer,
            setOdArchivePath,
            setCdcFilename,
            process,
            getIdentResponse,
            getEthMacAddr,
            checkKernelStack,
            waitSyncEvent,
            getVersion,
            getVersionString,
            getStackConfiguration,
            getStackInfo,
            getSocTime,

            allocProcessImage,
            freeProcessImage,
            linkProcessImageObject,
            exchangeProcessImageIn,
            exchangeProcessImageOut,
            getProcessImageIn,
            getProcessImageOut,

            setupProcessImage,

            triggerPresForward,

            getRetStr
        };

    private:
        using RawMessagePtr = MessageDispatcher::RawMessagePtr;

  protected:
    virtual void initialize();
    virtual void handleMessage(OPP::cMessage *rawMsg);

  private:
    void handleApiCall(RawMessagePtr msg);
    void handleEventReturn(RawMessagePtr msg);


    interface::api::ErrorType processEvent(interface::api::ApiEventType eventType_p,
                interface::api::ApiEventArg* pEventArg_p);

    static interface::api::ErrorType processEvent(interface::api::ApiEventType eventType_p,
            interface::api::ApiEventArg* pEventArg_p, void* pUserArg_p);


    void sendReturnMessage(OPP::cMessage* msg);

    // Member
  private:
    MessageDispatcher mDispatcher;
    interface::api::ApiFunctions mApi;
    OPP::cGate* mReturnGate;
    OPP::cGate* mEventGate;
};

#endif
