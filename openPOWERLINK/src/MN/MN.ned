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
    @display("bgb=348,401");
    submodules:
        edrv: Edrv {
            @display("p=55,202");
        }
        hresTimer: HresTimer {
            @display("p=55,58;is=s");
        }
        sdoUdp: SdoUdp {
            @display("p=55,348");
        }
        target: Target {
            @display("p=55,130");
        }
        trace: Trace {
            @display("p=55,271");
        }
        api: Api {
            @display("p=311,33");
        }
}
