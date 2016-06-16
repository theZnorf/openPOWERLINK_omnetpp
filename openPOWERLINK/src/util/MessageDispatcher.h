/*
 * MessageDispatcher.h
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

#include <functional>
#include <map>
#include <omnetpp.h>

class MessageDispatcher
{
        // Definitions
    public:
        using Id = int;
        using RawMessagePtr = OPP::cMessage*;
        using MessageFunc = std::function<void(RawMessagePtr)>;
        using DispatcherCont = std::map<Id, MessageFunc>;

        // C-Tor / D-Tor
    public:
        MessageDispatcher();
        virtual ~MessageDispatcher();

        // Methods
    public:
        void registerFunction(Id id, MessageFunc func);
        void registerFunction(OPP::cGate* gate, MessageFunc func);
        void dispatch(RawMessagePtr msg);

        // Member
    private:
        DispatcherCont mDispatch;
};

#endif /* MESSAGEDISPATCHER_H_ */
