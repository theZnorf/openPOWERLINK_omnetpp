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

#include "Led.h"

USING_NAMESPACE

Define_Module(Led);

void Led::initialize()
{
    // resolve parameter
    mActiveColor = par("activeColor");
    mInactiveColor = par("inactiveColor");
    const char* signalName = par("signalName");

    // register listener
    getParentModule()->subscribe(signalName, this);
}

void Led::receiveSignal(::cComponent* source, ::simsignal_t signalID, bool b)
{
    if (b)
        getDisplayString().setTagArg("b", 3, mActiveColor);
    else
        getDisplayString().setTagArg("b", 3, mInactiveColor);
}
