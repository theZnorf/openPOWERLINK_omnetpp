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

package openpowerlink.generic;

import openpowerlink.generic.stack.Stack;


module GenericNode
{
    @display("bgb=282,253");
    submodules:
        stack: Stack {
            @display("p=71,168;is=vl");
        }
        event: EventBase {
            @display("p=196,168");
        }
        app: AppBase {
            @display("p=196,61");
        }
        demoBase: DemoBase {
            @display("p=71,61");
        }
    connections:
        demoBase.apiCall --> stack.apiCall;
        stack.apiReturn --> demoBase.apiReturn;
        stack.event --> event.event;
        event.eventReturn --> stack.eventReturn;
        demoBase.appCall --> app.functionCall;
        app.return --> demoBase.appReturn;
        event.stackShutdown --> demoBase.stackShutdown;
}
