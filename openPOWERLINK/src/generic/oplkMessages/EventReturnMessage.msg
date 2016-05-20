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
    #include "ReturnMessage_m.h"
}}

class interface::api::ApiEventType extends void;
class interface::api::ApiEventArg extends void;
message oplkMessages::ReturnMessage;
    
cplusplus {{
    USING_NAMESPACE
}}

namespace oplkMessages;

message EventReturnMessage extends ReturnMessage {
    interface::api::ApiEventType eventType;
    interface::api::ApiEventArg eventArg;
}
