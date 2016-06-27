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
        output stackShutdown; // Output gate for occuring shutdown event
}
