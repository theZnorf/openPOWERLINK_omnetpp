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

#ifndef __OPENPOWERLINK_EVENTBASE_H_
#define __OPENPOWERLINK_EVENTBASE_H_

#include <omnetpp.h>
#include "ApiDef.h"

class EventBase : public OPP::cSimpleModule
{
        // Methods
  protected:
    virtual void initialize();
    virtual void handleMessage(OPP::cMessage *msg);

    virtual interface::api::ErrorType processEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);

    virtual interface::api::ErrorType processNmtStateChangeEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processErrorEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processWarningEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processHistoryEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processNodeEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processPdoChangeEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processCfmProgressEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);
    virtual interface::api::ErrorType processCfmResultEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg);

    // Member
  protected:
    OPP::cGate* mReturnGate;
    OPP::cGate* mShutdownGate;
    OPP::simsignal_t mEventTypeSignal;
    OPP::simsignal_t mNmtStateSignal;
    bool mPrintEventType;
};

#endif
