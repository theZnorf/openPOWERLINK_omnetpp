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

simple Edrv
{
    parameters:
        @display("t=Uninitialized,r,black;i=block/network,red");
        int numberOfInstances = default(1);
        string libName;
        
        @signal[sentEtherType];
        @signal[receivedEtherType];
        @signal[sentOplkMessageType];
        @signal[receivedOplkMessageType];
        
        @statistic[sentEtherTypes](title="Sent EtherTypes";record=count,vector;source=sentEtherType);
        @statistic[receivedEtherTypes](title="Received EtherTypes";record=count,vector;source=receivedEtherType);
        @statistic[sentOplkTypes](title="Sent Oplk Message Types";record=count,vector;source=sentOplkMessageType);
        @statistic[receivedOplkTypes](title="Received Oplk Message Types";record=count,vector;source=receivedOplkMessageType);
        
    gates:
        input ethernetIn;
        output ethernetOut;
}
