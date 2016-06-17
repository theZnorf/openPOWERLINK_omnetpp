//
// @author Franz Profelt
// 
// Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
//

package openpowerlink.generic;

//
// Interface for Demo modules used within a openPOWERLINK node derived from GenericNode
//
moduleinterface IDemo
{
    parameters:
        double startUpDelay @unit(ns); // Delay of simulation time after the demo should start initializing
        double mainLoopInterval @unit(ns);  // Interval of the main processing loop
        double shutdownTime @unit(ns); // Simulation time after the shutdown procedure should be initiated
        
    gates:
        output apiCall; // Output gate for Api calls
        input apiReturn; // Input gate for Api return messages
        output appCall; // Output gate for App calls
        input appReturn; // Input gate for App return messages
        input stackShutdown; // Input gate for shutdown message
}