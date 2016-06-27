/**
 ********************************************************************************
 \file   EventBase.h

 \brief  Include file for Base class of Event modules.

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#ifndef __OPENPOWERLINK_EVENTBASE_H_
#define __OPENPOWERLINK_EVENTBASE_H_

#include <omnetpp.h>
#include "ApiDef.h"

/**
 * \brief Base class for Event modules providing basic functionalities
 *
 * This class inherits form cSimpleModule and is implemented event based.
 * Dispatching to various virtual methods and output prints are implemented
 * within this class, which can either be subclassed or used directly.
 * A shutdown event results in an according message sent to the Demo module.
 */
class EventBase : public OPP::cSimpleModule
{
        // Methods
  protected:
    virtual void initialize();
    virtual void handleMessage(OPP::cMessage *msg);

    /**
     * \brief General Method for processing a received event
     *
     * This method dispatches the events according to its type to the different
     * methods.
     * Additionally this method emits the received event type as signal.
     */
    virtual interface::api::ErrorType processEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing an Nmt state change event
     */
    virtual interface::api::ErrorType processNmtStateChangeEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing an error event
     */
    virtual interface::api::ErrorType processErrorEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing a warning event
     *
     */
    virtual interface::api::ErrorType processWarningEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing a history event
     */
    virtual interface::api::ErrorType processHistoryEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing a node event
     */
    virtual interface::api::ErrorType processNodeEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing a PDO change event
     */
    virtual interface::api::ErrorType processPdoChangeEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing a Cfm progress event
     */
    virtual interface::api::ErrorType processCfmProgressEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    /**
     * \brief Virtual method for processing a Cfm result event
     */
    virtual interface::api::ErrorType processCfmResultEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg, void* userArg);

    // Member
  protected:
    OPP::cGate* mReturnGate;
    OPP::cGate* mShutdownGate;
    OPP::simsignal_t mEventTypeSignal;
    OPP::simsignal_t mNmtStateSignal;
    bool mPrintEventType;
};

#endif
