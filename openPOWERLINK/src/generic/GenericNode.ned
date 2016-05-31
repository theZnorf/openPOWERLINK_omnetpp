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
        @display("bgb=282,300");
        int numberOfInstances = default(1);
        string libName;

        string DemoType;
        string AppType;
        string EventType;

        double mainLoopInterval @unit(ns) = default(1ms);

        int numberOfApiConnections = default(2);

    gates:
        input ethernetIn;
        output ethernetOut;
        input udpIn;
        output udpOut;
    submodules:
        stack: Stack {
            @display("p=138,151");
            libName = libName;
            numberOfInstances = numberOfInstances;
            numberOfApiConnections = numberOfApiConnections;
        }
        event: <EventType> like IEvent {
            @display("p=217,51");
        }
        app: <AppType> like IApp {
            @display("p=52,51");
        }
        demo: <DemoType> like IDemo {
            @display("p=138,51");
            mainLoopInterval = mainLoopInterval;
        }
        nmtState: NmtState {
            @display("p=138,228");
        }
    connections:
        // Demo Api connection
        demo.apiCall --> stack.apiCall[0];
        stack.apiReturn[0] --> demo.apiReturn;

        // App Api connection
        app.apiCall --> stack.apiCall[1];
        stack.apiReturn[1] --> app.apiReturn;

        // Event connection
        stack.event --> event.event;
        event.eventReturn --> stack.eventReturn;
        event.stackShutdown --> demo.stackShutdown;

        // App - Demo connection
        demo.appCall --> app.functionCall;
        app.return --> demo.appReturn;

        // Network connections
        ethernetIn --> stack.ethernetIn;
        stack.ethernetOut --> ethernetOut;
        stack.udpOut --> udpOut;
        udpIn --> stack.udpIn;
}
