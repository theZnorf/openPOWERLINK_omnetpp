/**
 ********************************************************************************
 \file   DemoBase.h

 \brief  Include file for Base class of Demo modules.

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#ifndef __OPENPOWERLINK_DEMOBASE_H_
#define __OPENPOWERLINK_DEMOBASE_H_

#include <omnetpp.h>
#include "UseApiBase.h"
#include "ReturnMessage_m.h"
#include "MessageDispatcher.h"

/**
 * \brief Base class for Demo modules implementing basic functionalities.
 *
 * This class inherits from UseApiBase and represents a cSimpleModule which
 * sends messages to the Api module and blocks the according method calls
 * until reception of the return message.
 * Additionally this class implements the handling of various states and the
 * communication to IApp and IEvent modules.
 */
class DemoBase : public UseApiBase
{
        // Definitions
    private:
        using RawMessagePtr = MessageDispatcher::RawMessagePtr;

        enum class DemoState : short
        {
            undefined,
            initializePowerlink,
            initializeApp,
            swReset,
            mainloop,
            shuttingDownApp,
            shuttingDown
        };

        // C-Tor
    public:
        /**
         * \brief Default constructor initializing the base class with the
         * name of the Api sending gate
         */
        DemoBase();

        // Methods
    protected:
        virtual void initialize();
        virtual void handleOtherMessage(MessagePtr msg) override;
        virtual void handleSelfMessage(MessagePtr msg);

        /**
         * \brief Initialize the openPOWERLINK stack
         *
         * This pure virtual method is called during initialization state.
         */
        virtual void initPowerlink() = 0;

        /**
         * \brief Shutdown the openPOWERLINK stack
         *
         * This pure virtual method is called during shutdown state.
         */
        virtual void shutdownPowerlink() = 0;

    private:
        void processAppReturn(RawMessagePtr msg);
        void processStackShutdown(RawMessagePtr msg);

        // Member
    protected:
        OPP
        ::cGate* mApiCallGate;OPP
        ::cGate* mAppCallGate;

        MessageDispatcher mDispatcher;

        simtime_t mStartUpDelay;
        simtime_t mMainLoopInterval;

        simtime_t mShutdownTime;

        DemoState mState;
};

#endif
