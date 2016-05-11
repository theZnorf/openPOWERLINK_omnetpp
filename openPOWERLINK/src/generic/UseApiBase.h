/*
 * UseApiBase.h
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#ifndef USEAPIBASE_H_
#define USEAPIBASE_H_

#include <string>
#include <memory>
#include <map>
#include <omnetpp.h>
#include "ApiDef.h"
#include "Api.h"
#include "ReturnMessage_m.h"

class UseApiBase : public OPP::cSimpleModule
{
        // Definitions
    protected:
        using MessagePtr = std::shared_ptr<OPP::cMessage>;
        using RawMessagePtr = OPP::cMessage*;
        using ErrorType = interface::api::ErrorType;
        using Error = interface::api::Error;
        using CallType = Api::ApiCallType;
        using Kind = short;
        using ReturnCont = std::map<CallType, MessagePtr>;

        // C-Tor / D-Tor
    public:
        UseApiBase(std::string const & sendGateName);
        virtual ~UseApiBase();

        // Methods
    public:
        virtual void initialize();
        virtual void activity();

    protected:
        void initStack();
        void createStack(interface::api::ApiInitParam& param);
        void destroyStack();
        void exitStack();
        void execNmtCommand(interface::api::NmtEvent event);
        void cbGenericObdAccess(interface::api::ObdCallbackParam* param);
        void linkObject(UINT objIndex, void* var, UINT* varEntries, interface::api::ObdSize* entrySize, UINT firstSubIndex);
        void readObject(interface::api::SdoComConHdl* hdl, UINT nodeId, UINT index, UINT subIndex, void* dstDataLe, UINT* size, interface::api::SdoType sdoType, void* userArg);
        void writeObject(interface::api::SdoComConHdl* hdl, UINT nodeId, UINT index, UINT subIndex, void* srcDataLe, UINT size, interface::api::SdoType sdoType, void* userArg);
        void finishUserAccess(interface::api::ObdAlConnHdl* userObdConHdl);
        void enableUserAccess(bool enable);
        void freeSdoChannel(interface::api::SdoComConHdl sdoComConHdl);
        void abortSdo(interface::api::SdoComConHdl sdoComConHdl, UINT32 abortCode);
        void readLocalObject(UINT index, UINT subIndex, void* dstData, UINT* size);
        void writeLocalObject(UINT index, UINT subIndex, void* srcData, UINT size);
        void sendAsndFrame(UINT8 dstNodeId, interface::api::AsndFrame* asndFrame, size_t asndSize);
        void sendEthFrame(interface::api::PlkFrame* frame, UINT frameSize);
        void setAsndForward(UINT8 serviceId, interface::api::AsndFilter filterType);
        void setNonPlkForward(bool enable);
        void postUserEvent(void* userArg);
        void triggerMnStateChange(UINT nodeId, interface::api::NmtNodeCommand nodeCommand);
        void setCdcBuffer(BYTE* cdc, UINT cdcSize);
        void setCdcFilename(std::string const & fileName);
        void setOdArchivePath(std::string const & backUpPath);
        void stackProcess();
        void getIdentResponse(UINT nodeId, interface::api::IdentResponse ** identResponse);
        void getEthMacAddr(UINT8* macAddr);
        bool checkKernelStack();
        void waitSyncEvent(ULONG timeout);
        UINT32 getVersion();
        std::string getVersionString();
        UINT32 getStackConfiguration();
        void getStackInfo(interface::api::ApiStackInfo* stackInfo);
        void getSocTime(interface::api::SocTimeInfo* timeInfo);

        void allocProcessImage(UINT sizeProcessImageIn, UINT sizeProcessImageOut);
        void freeProcessImage();
        void linkProcessImageObject(UINT objIndex, UINT firstSubIndex, UINT offsetPI, bool outputPI, interface::api::ObdSize entrySize, UINT* varEntries);
        void exchangeProcessImageIn();
        void exchangeProcessImageOut();
        void* getProcessImageIn();
        void* getProcessImageOut();

        void setupProcessImage();

        void triggerPresForward(UINT nodeId);

    protected:
        virtual void handleOtherMessage(MessagePtr msg) = 0;

        static void checkReturnMessage(MessagePtr msg, std::string const & errorMessage);

        void receiveMessage();

        MessagePtr sendMessageWithCallTypeAndWaitForReturn(RawMessagePtr msg, CallType calltype);

        // Member
    private:
        std::string mSendGateName;
        OPP::cGate* mSendGate;
        ReturnCont mReturnValues;
};

#endif /* USEAPIBASE_H_ */
