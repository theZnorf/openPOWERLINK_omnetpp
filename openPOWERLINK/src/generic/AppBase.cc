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

#include "AppBase.h"
#include "MsgPtr.h"
#include "ApiDef.h"
#include "InitMessage_m.h"

USING_NAMESPACE

Define_Module(AppBase);

void AppBase::initialize()
{

}

void AppBase::handleMessage(cMessage *rawMsg)
{
    MsgPtr msg(rawMsg);

    if (msg != nullptr)
    {
        // check message kind
        switch (static_cast<AppCallType>(msg->getKind()))
        {
            case AppCallType::init:
                break;
            case AppCallType::processSync:
                break;
            case AppCallType::shutdown:
                break;
            default:
                error("unknown message kind %d", msg->getKind());
        }


    }
}
