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

import inet.common.LayeredProtocolBase;
import inet.linklayer.ethernet.EtherEncap;
import inet.linklayer.ethernet.EtherEncapDummy;
import inet.linklayer.ethernet.EtherMACFullDuplex;
import inet.linklayer.ethernet.EthernetInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.contract.INetworkLayer;
import inet.physicallayer.contract.packetlevel.IPhysicalLayer;
import openpowerlink.generic.stack.Stack;


module GenericNode
{
    parameters:
        @display("bgb=282,429");
        @networkNode();
    gates:
        //inout ethernetIf;
    submodules:
        stack: Stack {
            @display("p=71,268");
        }
        event: EventBase {
            @display("p=196,154");
        }
        app: AppBase {
            @display("p=71,53");
        }
        demo: DemoBase {
            @display("p=71,154");
        }
        nmtState: NmtState {
            @display("p=133,352");
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
}
