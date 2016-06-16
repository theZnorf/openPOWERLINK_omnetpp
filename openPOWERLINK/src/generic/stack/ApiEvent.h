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

#ifndef __OPENPOWERLINK_APIEVENT_H_
#define __OPENPOWERLINK_APIEVENT_H_

#include <omnetpp.h>
#include <SendAwaitedReturnBase.h>
#include "OplkApiEvent.h"

class ApiEvent : public SendAwaitedReturnBase<interface::api::ApiEventType>
{
        // Definitions
    public:
        using ApiEventType = interface::OplkApiEvent::ApiEventType;
        using ApiEventArg = interface::OplkApiEvent::ApiEventArg;

        // C-Tor
    public:
        ApiEvent();

        // Methods
    public:
        void eventCb(ApiEventType eventType, ApiEventArg* eventArg, void* userArg);

    protected:
        virtual void initialize();
        virtual void handleOtherMessage(MessagePtr msg) override;

        virtual void foo();

        // Static methods
    public:
        static void setEventType(RawMessagePtr msg, Kind kind);
        static Kind getEventType(RawMessagePtr msg);
};

#endif
