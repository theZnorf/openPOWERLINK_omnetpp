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

#include "ProcessSync.h"
#include "OplkProcessSync.h"
#include "MsgPtr.h"
#include "OplkException.h"
#include "AppBase.h"

USING_NAMESPACE

Define_Module(ProcessSync);

void ProcessSync::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkProcessSync::Instance numberOfInstances = par("numberOfInstances");

    // init stub
    interface::OplkProcessSync::setLibraryInfo(libName, numberOfInstances);
    interface::OplkProcessSync::getInstance().initModule(this);

    // resolve parameter
    mDefaultReturnValue = &par("defaultReturnValue");

    // resolve gate
    mSendGate = gate("processSync");
}

void ProcessSync::processSyncCb()
{
    Enter_Method("processSyncCb");

    bubble("process sync called");

    // send process sync message
    send(new cMessage("process sync", static_cast<short>(AppBase::AppBaseCallType::processSync)), mSendGate);

    // check defined default return value
    auto retVal = static_cast<interface::api::Error>(mDefaultReturnValue->longValue());
    if (retVal != interface::api::Error::kErrorOk)
        throw interface::OplkException("Default Process sync return", retVal);
}

void ProcessSync::handleMessage(cMessage* rawMsg)
{
    MsgPtr msg(rawMsg);

    // empty method
}
