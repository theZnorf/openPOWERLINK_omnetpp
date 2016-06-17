/**
 ********************************************************************************
 \file   AppBase.h

 \brief  Include file for Base class of App modules.

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#ifndef __OPENPOWERLINK_APPBASE_H_
#define __OPENPOWERLINK_APPBASE_H_

#include <omnetpp.h>
#include <vector>
#include "UseApiBase.h"
#include "ReturnHandler.h"

/**
 * \brief Base class for App modules implementing basic functionalities.
 *
 * This class inherits from UseApiBase and represents a cSimpleModule which
 * sends messages to the Api module and blocks the according method calls
 * until reception of the return message.
 * Addionally this class implements the message handling for communication with
 * other Modules and the according function handling.
 */
class AppBase : public UseApiBase
{
        // Definitions
    public:
        /**
         * \brief Types of functions available via messages
         */
        enum class AppBaseCallType
            : short
            {
                init = 0, shutdown, processSync
        };

    protected:
        using GateCont = std::vector<OPP::cGate*>;
        using HandlerCont = std::vector<HandlerPtr>;
        // C-Tor
    public:
        /**
         * \brief Default constructor initializing base class
         */
        AppBase();

        // Methods
    protected:
        virtual void initialize();
        virtual void handleOtherMessage(MessagePtr msg);

        /**
         * \brief Initialize Application
         *
         * This pure virtual method is called after receiving a message with
         * messageKind AppBaseCallType::init.
         */
        virtual interface::api::ErrorType initApp() = 0;

        /**
         * \brief Execute synchronous operations
         *
         * This pure virtual method is called after receiving a message with
         * messageKind AppBaseCallType::processSync.
         */
        virtual interface::api::ErrorType processSync() = 0;

        /**
         * \brief Shutdown Application
         *
         * This pure virtual method is called after receiving a message with
         * messageKind AppBaseCallType::init.
         */
        virtual void shutdownApp() = 0;

        /**
         * \brief Handler for messages with different kinds than AppBaseCallType.
         */
        virtual void handleAppMessage(MessagePtr msg);

        // Member
    private:
        GateCont mReturnGates;

    protected:
        HandlerCont mReturns;
};

#endif
