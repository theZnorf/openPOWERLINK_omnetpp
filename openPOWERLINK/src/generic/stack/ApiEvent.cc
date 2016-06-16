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

#include "ApiEvent.h"
#include "ApiMessages.h"
#include "debugstr.h"
#include "OplkException.h"
#include "OplkApiEvent.h"

USING_NAMESPACE

Define_Module(ApiEvent);

ApiEvent::ApiEvent():
        SendAwaitedReturnBase("event", ApiEvent::setEventType, ApiEvent::getEventType)
{
}

void ApiEvent::initialize()
{
    SendAwaitedReturnBase::initialize();

    // init interface
    interface::OplkApiEvent::getInstance().initModule(this);
}

void ApiEvent::eventCb(ApiEventType eventType, ApiEventArg* eventArg, void* userArg)
{
    //Enter_Method("eventCb");

    if (eventType == interface::api::ApiEvent::kOplkApiEventCriticalError)
        EV << "Critical error occured" << endl;

    if (eventType == interface::api::ApiEvent::kOplkApiEventNmtStateChange)
    {
        EV << "State change occurred" << endl;
    }

    // create event message
    auto eventMsg = new oplkMessages::EventMessage();
    eventMsg->setName(("event - " + std::string(interface::debug::getApiEventStr(eventType))).c_str());
    eventMsg->setEventArg(*eventArg);
    eventMsg->setUserArg((oplkMessages::PointerCont)userArg);

    // send message with awaited return
    sendAwaitedMessage(eventMsg, eventType);

    // wait for the reception of the according return value
    auto msg = waitForReturnMessage(eventType);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::EventReturnMessage*>(msg.get());

    if ((retMsg != nullptr) && (retMsg->getReturnValue() != interface::api::Error::kErrorOk))
        throw interface::OplkException("EventCb return value", retMsg->getReturnValue());
}

void ApiEvent::handleOtherMessage(MessagePtr msg)
{
}


void ApiEvent::foo()
{
    EV << "Foo called" << std::endl;
}

void ApiEvent::setEventType(RawMessagePtr msg, Kind kind)
{
    // cast message
    auto eventMsg = dynamic_cast<oplkMessages::EventMessage*>(msg);

    if (eventMsg != nullptr)
    {
        // set event type
        eventMsg->setEventType(kind);
    }
    else
        throw std::runtime_error("invalid message type");
}

ApiEvent::Kind ApiEvent::getEventType(RawMessagePtr msg)
{
    // cast message
    auto eventMsg = dynamic_cast<oplkMessages::EventReturnMessage*>(msg);

    if (eventMsg != nullptr)
    {
        // get event type
        return eventMsg->getEventType();
    }
    return -1;
}
