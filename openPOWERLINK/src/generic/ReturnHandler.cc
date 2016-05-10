/*
 * ReturnHandler.cc
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#include <stdexcept>
#include "ReturnHandler.h"
#include "ReturnMessage_m.h"

USING_NAMESPACE

ReturnHandler::ReturnHandler(SendFunc sendFunc) :
        mSend(sendFunc)
{
}

ReturnHandler::~ReturnHandler()
{
}

void ReturnHandler::sendReturnValue(interface::api::ErrorType returnValue)
{
    // create return message
    auto msg = new oplkMessages::ReturnMessage();
    msg->setReturnValue(returnValue);

    sendReturnMessage(msg);
}

void ReturnHandler::sendReturnValue(interface::api::ErrorType returnValue, Kind kind)
{
    // create return message
    auto msg = new oplkMessages::ReturnMessage();
    msg->setReturnValue(returnValue);
    msg->setKind(kind);

    sendReturnMessage(msg);
}

void ReturnHandler::sendReturnMessage(cMessage* msg)
{
    if (msg == nullptr)
        throw std::invalid_argument("invalid message");

    mSend(msg);
}

void ReturnHandler::sendReturnMessage(cMessage* msg, Kind kind)
{
    if (msg == nullptr)
        throw std::invalid_argument("invalid message");

    msg->setKind(kind);

    sendReturnMessage(msg);
}
