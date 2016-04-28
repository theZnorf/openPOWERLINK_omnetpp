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

package openpowerlink.MN;

module MN
{
    @display("bgb=394,132");
    submodules:
        edrv: Edrv {
            @display("p=193,58");
        }
        hresTimer: HresTimer {
            @display("p=55,58;is=s");
        }
        sdoUdp: SdoUdp {
            @display("p=263,58");
        }
        target: Target {
            @display("p=125,58");
        }
        trace: Trace {
            @display("p=331,58");
        }
}
