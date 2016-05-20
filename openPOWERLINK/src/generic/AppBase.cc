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

    // resolve gate
    mReturnGate = gate("return");

    // create return handler with send method bound to this and the return gate
    mRet = std::make_unique < ReturnHandler
            > (std::bind(static_cast<int (AppBase::*)(cMessage*, cGate*)>(&AppBase::send), this, std::placeholders::_1, mReturnGate));
}

void AppBase::handleOtherMessage(MessagePtr msg)
{
    if (msg != nullptr)
    {
        auto ret = -1;

        // check message kind
        switch (static_cast<AppCallType>(msg->getKind()))
        {
            case AppCallType::init:
                ret = this->initApp();
                break;
            case AppCallType::processSync:
                ret = this->processSync();
                break;
            case AppCallType::shutdown:
                this->shutdownApp();
                break;
            default:
                error("unknown message kind %d", msg->getKind());
        }

        // check if return value was set
        if (ret != -1)
            mRet->sendReturnValue(ret, msg->getKind());
    }
}
