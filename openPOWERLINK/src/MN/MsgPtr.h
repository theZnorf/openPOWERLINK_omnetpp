/*
 * MessagePointer.h
 *
 *  Created on: Apr 21, 2016
 *      Author: franz
 */

#ifndef MSGPTR_H_
#define MSGPTR_H_

#include <memory>
#include <omnetpp.h>

using MsgPtr = std::unique_ptr<cMessage>;

#endif /* MSGPTR_H_ */
