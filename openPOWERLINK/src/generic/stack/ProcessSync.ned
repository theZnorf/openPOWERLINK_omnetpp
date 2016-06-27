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

simple ProcessSync
{
    parameters:
        int numberOfInstances = default(1); // Number of Instances of ProcessSync modules, should be set by parent module/network
        string libName; // Filename of shared library, should be set by parent module/network
        
        volatile int defaultReturnValue = default(0); // Default return value for the process sync cb

    gates:
        output processSync; // Output gate for process sync messaes forwarded to the app module IApp
}
