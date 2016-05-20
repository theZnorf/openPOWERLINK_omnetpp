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
    parameters:
        @display("bgb=282,347");
        
        string DemoType;
        string AppType;
        string EventType;
        
    gates:
        input ethernetIn;
        output ethernetOut;
        input udpIn;
        output udpOut;
    submodules:
        stack: Stack {
            @display("p=71,268");
        }
        event: <EventType> like IEvent {
            @display("p=196,154");
        }
        app: <AppType> like IApp {
            @display("p=71,53");
        }
        demo: <DemoType> like IDemo {
            @display("p=71,154");
        }
        nmtState: NmtState {
            @display("p=157,268");
        }
    connections:
        demo.apiCall --> stack.apiCall;
        stack.apiReturn --> demo.apiReturn;
        stack.event --> event.event;
        event.eventReturn --> stack.eventReturn;
        demo.appCall --> app.functionCall;
        app.return --> demo.appReturn;
        event.stackShutdown --> demo.stackShutdown;
        demo.appApiReturn --> app.apiReturn;
        app.apiCall --> demo.appApiCall;
        ethernetIn --> stack.ethernetIn;
        stack.ethernetOut --> ethernetOut;
        stack.udpOut --> udpOut;
        udpIn --> stack.udpIn;
}
