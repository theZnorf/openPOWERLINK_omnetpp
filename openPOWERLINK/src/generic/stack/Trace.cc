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

#include "Trace.h"
#include "interface/OplkTrace.h"

USING_NAMESPACE

Define_Module(Trace);

void Trace::initialize()
{
    interface::OplkTrace::getInstance().initModule(this);
}

void Trace::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void Trace::trace(std::string msg)
{
    EV << "TRACE: " << msg << std::endl;
    bubble(msg.c_str());
}
