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

class UseApiBase : public OPP::cSimpleModule
{
        // Definitions
    protected:
        using MessagePtr = std::shared_ptr<OPP::cMessage>;
        using ErrorType = interface::api::ErrorType;
        using Error = interface::api::Error;
        using CallType = Api::ApiCallType;
        using ReturnCont = std::map<CallType, ErrorType>;

        static const ErrorType cInvalidError = -1;

        // C-Tor / D-Tor
    public:
        UseApiBase(std::string const & sendGateName);
        virtual ~UseApiBase();

        // Methods
    public:
        virtual void initialize();
        virtual void activity();

        void initStack();
        void createStack(interface::api::ApiInitParam& param);
        void setCdcFile(std::string const & fileName);

    protected:
        virtual void handleOtherMessage(MessagePtr msg) = 0;

        static void checkReturnValue(ErrorType err, std::string const & msg);

        void receiveMessage();

        // Member
    private:
        std::string mSendGateName;
        OPP::cGate* mSendGate;
        ReturnCont mReturnValues;
};

#endif /* USEAPIBASE_H_ */
