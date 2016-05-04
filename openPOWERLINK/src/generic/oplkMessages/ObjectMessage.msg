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

cplusplus {{
    #include "stack/interface/ApiDef.h"
    #include "OplkPacking.h"
}}

class interface::api::SdoComConHdl extends void;
class interface::api::SdoType extends void;
class oplkMessages::PointerCont extends void;
    
cplusplus {{
    USING_NAMESPACE
}}

namespace oplkMessages;

message ObjectMessage {
    interface::api::SdoComConHdl sdoComConHdl;
    unsigned int nodeId;
    unsigned int index;
    unsigned int subIndex;
    unsigned int size;
    interface::api::SdoType sdoType;
    PointerCont userArg;
}
