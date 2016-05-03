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

#include "EventBase.h"
#include "EventMessage_m.h"
#include "ReturnMessage_m.h"
#include "MsgPtr.h"

using namespace std;
USING_NAMESPACE

Define_Module(EventBase);

void EventBase::initialize()
{
    // resolve return gate
    mReturnGate = gate("eventReturn");
    if (mReturnGate == nullptr)
        error("%s - unable to resolve gate", __PRETTY_FUNCTION__);
}

void EventBase::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        // cast to EventMessage
        auto eventMsg = dynamic_cast<oplkMessages::EventMessage*>(msg.get());

        if (eventMsg != nullptr)
        {
            // process Event
            auto ret = processEvent(eventMsg->getEventType(), eventMsg->getEventArg());

            // send return value
            sendReturnMessage(ret);
        }
    }
}

interface::api::ErrorType EventBase::processEvent(interface::api::ApiEventType eventType, interface::api::ApiEventArg eventArg)
{
    EV << "Process event:" << endl;
    EV << " EventType: " << eventType << endl;

    return interface::api::Error::kErrorOk;
}

void EventBase::sendReturnMessage(interface::api::ErrorType returnValue)
{
    auto retMsg = new oplkMessages::ReturnMessage();
    retMsg->setReturnValue(returnValue);

    send(retMsg, mReturnGate);
}
