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

#include "NmtState.h"
#include "debugstr.h"

USING_NAMESPACE

Define_Module(NmtState);

void NmtState::initialize()
{
    // subscribe listener to nmt state signal
    getParentModule()->subscribe("nmtState", this);
}

void NmtState::receiveSignal(::cComponent* source, ::simsignal_t signalID, unsigned long l)
{
    getDisplayString().setTagArg("t", 0, interface::debug::getNmtStateStr(static_cast<interface::api::NmtState>(l)));
}
