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
        @display("bgb=527,238,,,0;i=plk,,0;is=n");
        int numberOfInstances = default(1);
        string libName;
        string edrvType = default("DirectEdrv");
    gates:
        input apiCall[];
        output apiReturn[];
        output processSync;
        output event;

        input ethernetIn;
        output ethernetOut;
        input udpIn;
        output udpOut;
    submodules:
        api: Api {
            @display("p=187,49");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        edrv: <edrvType> like IEdrv {
            @display("p=90,191");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        hresTimer: HresTimer {
            @display("p=261,190");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        sdoUdp: SdoUdp {
            @display("p=171,191");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        target: Target {
            @display("p=422,197");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        trace: Trace {
            @display("p=343,191");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        process: ProcessSync {
            @display("p=259,49");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        apiEvent: ApiEvent {
            @display("p=330,49");
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

		// Process sync (App)
        process.processSync --> processSync;

        // Network
        edrv.ethernetOut --> ethernetOut;
        ethernetIn --> edrv.ethernetIn;
        udpIn --> sdoUdp.udpIn;
        sdoUdp.udpOut --> udpOut;
}
