//
// @author Franz Profelt
// 
// Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
//

package openpowerlink.generic;

import openpowerlink.generic.stack.Stack;

//
// Compound module representing a base class for every specialized openPOWERLINK
// node. Connecting the interfaces IDemo, IApp, IEvent with the Stack module.
//
module GenericNode
{
    parameters:
        @display("bgb=282,300");
        int numberOfInstances = default(1); // Number of instances
        string libName; // Name of the used shared library

        string DemoType; // Typename of the demo instance implementing IDemo
        string AppType; // Typename of the app instance implementing IApp
        string EventType; // Typename of the event instance implementing IEvent

        double mainLoopInterval @unit(ns) = default(1ms); // Interval of the main loop

    gates:
        input ethernetIn; // Input gate for ethernet connection
        output ethernetOut; // Output gate for ethernet connection
        input udpIn; // Input gate for UDP connetion
        output udpOut; // Output gate for UDP connection
        
    submodules:
        stack: Stack {
            @display("p=138,151");
            libName = libName;
            numberOfInstances = numberOfInstances;
        }
        event: <EventType> like IEvent {
            @display("p=217,51");
        }
        app: <AppType> like IApp {
            @display("p=52,51");
        }
        demo: <DemoType> like IDemo {
            @display("p=138,51");
            mainLoopInterval = mainLoopInterval;
        }
        nmtState: NmtState {
            @display("p=138,235");
        }
        status: Led {
            @display("p=197,151");
            signalName = "statusLed";
            activeColor = "green";
        }
        error: Led {
            @display("p=238,151");
            signalName = "errorLed";
            activeColor = "red";
        }
    connections:
		// Api connections
        demo.apiCall --> stack.apiCall++;
        stack.apiReturn++ --> demo.apiReturn;
        app.apiCall --> stack.apiCall++;
        stack.apiReturn++ --> app.apiReturn;
        
        // App call
        demo.appCall --> app.functionCall++;
        app.functionReturn++ --> demo.appReturn;
        
        // Process sync
        stack.processSync --> app.functionCall++;
        
        // Event
        stack.event --> event.event;
        event.stackShutdown --> demo.stackShutdown;


        // Network connections
        ethernetIn --> stack.ethernetIn;
        stack.ethernetOut --> ethernetOut;
        stack.udpOut --> udpOut;
        udpIn --> stack.udpIn;
}
