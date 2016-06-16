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

package openpowerlink.generic.stack;

module Stack
{
    parameters:
        @display("bgb=248,475,,,0;i=plk,,0;is=n");
        int numberOfInstances = default(1);
        string libName;
        string edrvType = default("DirectEdrv");
    gates:
        input apiCall[];
        output apiReturn[];
        output processSync;
        input processSyncReturn;
        output event;
        input eventReturn;

        input ethernetIn;
        output ethernetOut;
        input udpIn;
        output udpOut;
    submodules:
        api: Api {
            @display("p=51,48");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        edrv: <edrvType> like IEdrv {
            @display("p=38,144");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        hresTimer: HresTimer {
            @display("p=38,210");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        sdoUdp: SdoUdp {
            @display("p=38,268");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        target: Target {
            @display("p=38,331");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        trace: Trace {
            @display("p=38,389");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        process: ProcessSync {
            @display("p=123,48");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        apiEvent: ApiEvent {
            @display("p=201,48");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
    connections:

		// Api connections
        for i=0..sizeof(apiCall)-1 {
            apiCall[i] --> api.functionCall++;
        }

        for i=0..sizeof(apiReturn)-1 {
            api.functionCallReturn++ --> apiReturn[i];
        }

		// Event
        apiEvent.event --> event;
        eventReturn --> apiEvent.eventReturn;

		// Process sync (App)
        process.processSync --> processSync;
        processSyncReturn --> process.processSyncReturn;

        // Network
        edrv.ethernetOut --> ethernetOut;
        ethernetIn --> edrv.ethernetIn;
        udpIn --> sdoUdp.udpIn;
        sdoUdp.udpOut --> udpOut;
}
