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

package openpowerlink;

import openpowerlink.CN.CN;
import openpowerlink.MN.MN;


network singleCn
{
    parameters:
        @display("bgb=323,249");
        int numberOfInstances = default(2);
        string libraryPath = default("../openPOWERLINK_V2/stack/lib/linux/x86_64/");
        string mnLibName = default("liboplkmn-sim_d");
        string cnLibName = default("liboplkcn-sim_d");
    submodules:
        mn: MN {
            @display("p=79,114");
            numberOfInstances = numberOfInstances;
            libName = libraryPath + mnLibName;
            mainLoopInterval = 10us;
        }
        cn: CN {
            @display("p=176,114");
            numberOfInstances = numberOfInstances;
            libName = libraryPath + cnLibName;
            mainLoopInterval = 10us;
        }
    connections:

        cn.ethernetOut --> mn.ethernetIn;
        mn.ethernetOut --> cn.ethernetIn;
        cn.udpOut --> mn.udpIn;
        mn.udpOut --> cn.udpIn;
}
