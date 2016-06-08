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

#include <functional>
#include "AppBase.h"

USING_NAMESPACE

AppBase::AppBase() :
        UseApiBase("apiCall")
{
}

void AppBase::initialize()
{
    UseApiBase::initialize();

    for (auto i = 0; i < gateSize("functionReturn"); i++)
    {
        // resolve gates
        mReturnGates.push_back(gate("functionReturn", i));

        // create return handler with send method bound to this and the return gate
        mReturns.emplace_back(new ReturnHandler(std::bind(static_cast<int (AppBase::*)(cMessage*, cGate*)>(&AppBase::send),
                        this, std::placeholders::_1, mReturnGates[i])));
    }
}

void AppBase::handleOtherMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        interface::api::ErrorType ret = interface::api::Error::kErrorOk;

        // check message kind
        switch (static_cast<AppBaseCallType>(msg->getKind()))
        {
            case AppBaseCallType::init:
                ret = this->initApp();
                break;
            case AppBaseCallType::processSync:
                ret = this->processSync();
                break;
            case AppBaseCallType::shutdown:
                this->shutdownApp();
                break;
            default:
                handleAppMessage(msg);
        }

        // send return message
        mReturns.at(msg->getArrivalGate()->getIndex())->sendReturnValue(ret, msg->getKind());
    }
}

void AppBase::handleAppMessage(MessagePtr msg)
{
    // Empty function stub within base class
}
