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

void ApiEvent::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkApiEvent::Instance numberOfInstances = par("numberOfInstances");

    // init stub
    interface::OplkApiEvent::setLibraryInfo(libName, numberOfInstances);
    interface::OplkApiEvent::getInstance().initModule(this);

    // resolve parameter
    mReturnValues = &par("returnValues");

    // resolve gate
    mSendGate = gate("event");
}

void ApiEvent::eventCb(ApiEventType eventType, ApiEventArg* eventArg, void* userArg)
{
    Enter_Method("eventCb");

    interface::api::ErrorType retVal;

    // check if event type is defined in return values
    auto child = mReturnValues->xmlValue()->getFirstChildWithAttribute(nullptr, "eventType", std::to_string(eventType).c_str());
    if (child == nullptr)
    {
        auto childs = mReturnValues->xmlValue()->getChildrenByTagName("default");
        if (!childs.empty())
        {
            auto nodeStr = childs.at(0)->getNodeValue();
            auto nodeVal = std::atoi(nodeStr);
            retVal = static_cast<interface::api::ErrorType>(nodeVal);
        }
    }
    else
    {
        retVal = static_cast<interface::api::ErrorType>(std::atoi(child->getNodeValue()));
    }


    if (eventType == interface::api::ApiEvent::kOplkApiEventCriticalError)
        EV << "Critical error occured" << endl;

    if (eventType == interface::api::ApiEvent::kOplkApiEventNmtStateChange)
    {
        EV << "State change occurred" << endl;
    }

    // create event message
    auto eventMsg = new oplkMessages::EventMessage();
    eventMsg->setName(("event - " + std::string(interface::debug::getApiEventStr(eventType))).c_str());
    eventMsg->setEventType(eventType);
    eventMsg->setEventArg(*eventArg);
    eventMsg->setUserArg((oplkMessages::PointerCont)userArg);

    // send event message
    send(eventMsg, mSendGate);

    if (retVal != interface::api::Error::kErrorOk)
        throw interface::OplkException("Event cb return", retVal);
}

void ApiEvent::handleMessage(cMessage* rawMsg)
{
}
