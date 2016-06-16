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

USING_NAMESPACE

Define_Module(ProcessSync);

ProcessSync::ProcessSync()
    : SendAwaitedReturnBase("processSync")
{
}

void ProcessSync::initialize()
{
    SendAwaitedReturnBase::initialize();

    // init interface
    interface::OplkProcessSync::getInstance().initModule(this);
}

void ProcessSync::processSyncCb()
{
    Enter_Method("processSyncCb");

    std::cout << "PROCESS SYNC CALLED" << std::endl;

    // send awaited message
    sendAwaitedMessage(new cMessage("process sync"), AppBase::AppBaseCallType::processSync);

    auto msg = waitForReturnMessage(AppBase::AppBaseCallType::processSync);

    // cast message
    auto retMsg = dynamic_cast<oplkMessages::ReturnMessage*>(msg.get());

    if (retMsg != nullptr)
    {
        // check return value and throw exception when not ok
        auto ret = retMsg->getReturnValue();
        if (ret != interface::api::Error::kErrorOk)
            throw interface::OplkException("Process sync error", ret);
    }
}

void ProcessSync::handleOtherMessage(MessagePtr msg)
{
    // Empty method
}
