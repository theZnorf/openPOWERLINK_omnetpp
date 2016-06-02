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

#include <algorithm>
#include <functional>
#include <iostream>
#include <cctype>
#include "Trace.h"
#include "interface/OplkTrace.h"

USING_NAMESPACE

Define_Module(Trace);

void Trace::initialize()
{
    // resolve library info parameter
    std::string libName = par("libName");
    interface::OplkTrace::Instance numberOfInstances = par("numberOfInstances");

    // init stub
    interface::OplkTrace::setLibraryInfo(libName, numberOfInstances);
    interface::OplkTrace::getInstance().initModule(this);
}

void Trace::handleMessage(cMessage *msg)
{
    // empty method, no message communication supported
}

void Trace::trace(std::string msg)
{
    msg.erase(std::remove_if(msg.begin(), msg.end(), [](char c)
    {   return !std::isprint(c);}), msg.end());

    std::cout << "TRACE - " << getParentModule()->getParentModule()->getName() << " : " << msg << std::endl;

    //EV << getParentModule()->getParentModule()->getName() << " - " << msg.c_str() << "\n";

    bubble(msg.c_str());
}
