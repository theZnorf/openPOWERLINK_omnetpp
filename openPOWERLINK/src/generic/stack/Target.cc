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

#include <string>
#include <sstream>
#include "Target.h"
#include "interface/OplkTarget.h"
#include "interface/ApiDef.h"

using namespace std;
USING_NAMESPACE

Define_Module(Target);


void Target::initialize()
{
    interface::OplkTarget::getInstance().initModule(this);

    refreshDisplay();

    // resolver parameter
    mPrintCurrentTick = par("printCurrentTick");
}

void Target::handleMessage(cMessage *msg)
{
    // empty method, no messages used
}

void Target::msleep(MilliSecondsType milliseconds)
{
    bubble(("msleep called for waiting " + to_string(milliseconds) + " milliseconds").c_str());
}

void Target::setIpAddress(std::string interfaceName, AddressType ipAddress, AddressType subnetMask, MtuType mtu)
{
    bubble("setIpAddress called");
}

void Target::setDefaultGateway(AddressType defaultGateway)
{
    bubble("setDefaultGateway called");
}

Target::TickType Target::getTickCount()
{
    auto currTickTime = simTime().inUnit(SimTimeUnit::SIMTIME_MS);

    if (mPrintCurrentTick)
        EV <<"Current Tick: " << currTickTime << std::endl;

    return static_cast<TickType>(currTickTime);
}

void Target::setLed(LedType ledType, bool ledOn)
{
    bubble("setLed called");

    switch(ledType)
    {
        case interface::api::LedType::kLedTypeError:
            mErrorLed = ledOn;
            break;
        case interface::api::LedType::kLedTypeStatus:
            mStatusLed = ledOn;
            break;
        default:
            error("invalid led type: %d", ledType);
            break;

        refreshDisplay();
    }
}

void Target::refreshDisplay()
{
    std::stringstream strStream;

    strStream << "StatusLed: " << (mStatusLed? "On" : "Off") << std::endl;
    strStream << "ErrorLed: " << (mErrorLed? "On" : "Off");

    getDisplayString().setTagArg("t",0,strStream.str().c_str());
}
