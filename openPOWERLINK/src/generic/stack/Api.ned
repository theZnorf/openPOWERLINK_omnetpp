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


package openpowerlink.generic.stack;

// Module representing and providing the openPOWERLINK API within the openPOWERLINK Stack module.
// The different API functions are invoked via a cMessage or the according specific message from oplkMessages
// with the according Api::ApiCallType as Type.
simple Api
{
    parameters:
        int numberOfInstances = default(1); // Number of Instances of Api modules, should be set by parent module/network
        string libName; // Filename of shared library, should be set by parent module/network
        
        int numberOfFunctionalConnections = default(3); // Number of functional connections using the Api
        
        @signal[invokedFunctionType](type="short");
        @statistic[apiFunctions](title="Invoked API functions";record=count,vector;source=invokedFunctionType);
    
    gates:
        input functionCall[numberOfFunctionalConnections]; // Input vector for functional connections invoking the according API function
        output functionCallReturn[numberOfFunctionalConnections]; // Return vector for return messages of invoked API functions
        output event; // Output gate for event messages forwarded to the event module IEvent
        input eventReturn; // Input gate for the return messages from the event module IEvent
        output app; // Output gate for process sync messaes forwarded to the app module IApp
        input appReturn // Input gate for the return messages from the app module IApp
}
