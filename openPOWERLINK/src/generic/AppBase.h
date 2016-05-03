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

#ifndef __OPENPOWERLINK_APPBASE_H_
#define __OPENPOWERLINK_APPBASE_H_

#include <omnetpp.h>

/**
 * TODO - Generated class
 */
class AppBase : public OPP::cSimpleModule
{
        // Definitions
    public:
        enum class AppCallType : short
        {
            init,
            shutdown,
            processSync
        };

  protected:
    virtual void initialize();
    virtual void handleMessage(OPP::cMessage *msg);

    // Member
  private:
    OPP::cGate* mApiCallGate;
    OPP::cGate* mEventGate;
    int mApiReturnId;
    int mEventReturnId;
};

#endif
