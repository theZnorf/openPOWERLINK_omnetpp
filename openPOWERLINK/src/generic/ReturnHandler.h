/*
 * ReturnHandler.h
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#ifndef RETURNHANDLER_H_
#define RETURNHANDLER_H_

#include <memory>
#include <functional>
#include <omnetpp.h>
#include "ApiDef.h"

class ReturnHandler;
using HandlerPtr = std::unique_ptr<ReturnHandler>;

class ReturnHandler
{
        // Definitions
    public:
        using SendFunc = std::function<void(OPP::cMessage*)>;
        using Kind = short;

        // C-Tor / D-Tor
    public:
        ReturnHandler(SendFunc sendFunc);
        virtual ~ReturnHandler();

        // Methods
    public:
        void sendReturnValue(interface::api::ErrorType returnValue);
        void sendReturnValue(interface::api::ErrorType returnValue, Kind kind);
        void sendReturnMessage(OPP::cMessage* msg);
        void sendReturnMessage(OPP::cMessage* msg, Kind kind);

        // Member
    private:
        SendFunc mSend;
};

#endif /* RETURNHANDLER_H_ */
