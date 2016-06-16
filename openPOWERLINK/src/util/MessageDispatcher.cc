/*
 * MessageDispatcher.cc
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#include <MessageDispatcher.h>

USING_NAMESPACE

MessageDispatcher::MessageDispatcher()
{
}

MessageDispatcher::~MessageDispatcher()
{
}

void MessageDispatcher::registerFunction(MessageDispatcher::Id id, MessageDispatcher::MessageFunc func)
{
    mDispatch[id] = func;
}

void MessageDispatcher::registerFunction(::cGate* gate, MessageDispatcher::MessageFunc func)
{
    mDispatch[gate->getId()] = func;
}

void MessageDispatcher::dispatch(RawMessagePtr msg)
{
    mDispatch.at(msg->getArrivalGateId())(msg);
}
