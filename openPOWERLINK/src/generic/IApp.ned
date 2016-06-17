//
// @author Franz Profelt
// 
// Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
//

package openpowerlink.generic;

//
// Interface for App modules used within a openPOWERLINK node derived from GenericNode
//
moduleinterface IApp
{
    gates:
        input functionCall[]; // Input vector for each connection using App functions
        output functionReturn[]; // Output vector for return values of incomming app functions
        output apiCall; // Output gate for Api calls
        input apiReturn; // Input gate for Api return messages
}
