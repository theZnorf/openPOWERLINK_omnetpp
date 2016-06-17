//
// @author Franz Profelt
// 
// Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
//

package openpowerlink.generic;

//
// Interface for Event modules used within a openPOWERLINK node derived from GenericNode
//
moduleinterface IEvent
{
    gates:
        input event; // Input gate for occuring events
        output eventReturn;  // Output gate for return values from event handling
        output stackShutdown; // Output gate for occuring shutdown event
}
