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

//
// Simple module representing a single led which color can be set via parameter and the value is
// provided by a bool signal of its parent module
//
simple Led
{
    parameters:
        string signalName; // Name of the signal to subscribe within scope of parent module
        string activeColor = default("green"); // Fill color in active state
        string inactiveColor = default("grey50"); // Fill color in inactive state
        int diameter = default(30); // Diameter of Leds shape
        string borderColor = default("black"); // Border color of shape
        int borderWidth = default(1); // Borderwidth of shape
        
        @display("b=$diameter,$diameter,oval,$inactiveColor,$borderColor,$borderWidth");
}
