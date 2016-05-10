/*
 * UseApiBase.h
 *
 *  Created on: May 10, 2016
 *      Author: franz
 */

#ifndef USEAPIBASE_H_
#define USEAPIBASE_H_

#include <string>
#include <memory>
#include <omnetpp.h>
#include "ApiDef.h"

class UseApiBase : public OPP::cSimpleModule
{
        // Definitions
    protected:
        using MessagePtr = std::shared_ptr<OPP::cMessage>;

        // C-Tor / D-Tor
    public:
        UseApiBase(std::string const & sendGateName);
        virtual ~UseApiBase();

        // Methods
    public:
        virtual void activity();

    protected:
        virtual void handleOtherMessage(MessagePtr msg) = 0;



        // Member
    private:
        OPP::cGate* mSendGate;
};

#endif /* USEAPIBASE_H_ */
