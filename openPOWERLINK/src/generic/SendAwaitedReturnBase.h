/*
 * SendAwaitedReturnBase.h
 *
 *  Created on: May 19, 2016
 *      Author: franz
 */

#ifndef SENDAWAITEDRETURNBASE_H_
#define SENDAWAITEDRETURNBASE_H_

#include <exception>
#include <string>
#include <functional>
#include <memory>
#include <map>

#include <omnetpp.h>

template<typename TKind>
class SendAwaitedReturnBase : public OPP::cSimpleModule
{
        // Definitions
    protected:
        using Kind = TKind;
        using RawMessagePtr = OPP::cMessage*;
        using MessagePtr = std::shared_ptr<OPP::cMessage>;
        using MessageCont = std::map<Kind, MessagePtr>;
        using ApplyKindFunc = std::function<void(RawMessagePtr, Kind)>;
        using GetKind = std::function<Kind(RawMessagePtr)>;

        // C-Tor
    protected:
        SendAwaitedReturnBase(std::string const & sendGateName, ApplyKindFunc applyKind, GetKind getKind) :
                cSimpleModule(32000), mSendGateName(sendGateName), mApplyKind(applyKind), mGetKind(getKind)
        {
        }

        virtual ~SendAwaitedReturnBase()
        {

        }

        // Methods
    protected:
        virtual void initialize()
        {
            // resolve gate
            mSendGate = gate(mSendGateName.c_str());
        }

        virtual void activity()
        {
            mRunning = true;

            while (mRunning)
            {
                // receive message
                receiveMessage();
            }
        }

        void receiveMessage()
        {
            // receive message
            auto rawMsg = receive();

            if (rawMsg != nullptr)
            {
                if (rawMsg->getArrivalModule() == this)
                {
                    MessagePtr msg(rawMsg);

                    // get kind from message
                    auto kind = mGetKind(msg.get());

                    // check if kind is awaited
                    if (mMsgCont.find(kind) != mMsgCont.end())
                    {
                        mMsgCont[kind] = msg;
                    }
                    else
                        // forward unexpected message
                        handleOtherMessage(msg);
                }
            }
        }

        void sendAwaitedMessage(RawMessagePtr msg, Kind kind, OPP::cGate* sendGate)
        {
            // apply kind to message
            mApplyKind(msg, kind);

            // add awaited kind to container and set to nullptr
            mMsgCont[kind] = nullptr;

            // send message via sendgate
            send(msg, sendGate);
        }

        void sendAwaitedMessage(RawMessagePtr msg, Kind kind)
        {
            sendAwaitedMessage(msg, kind, mSendGate);
        }

        MessagePtr waitForReturnMessage(Kind kind)
        {
            // check if kind is not awaited
            if (mMsgCont.find(kind) == mMsgCont.end())
                return nullptr;

            // check if message for given kind was already received
            while (mMsgCont[kind] == nullptr)
                // receive message
                receiveMessage();

            // get received message
            MessagePtr msg = mMsgCont[kind];

            // remove expected kind
            mMsgCont.erase(kind);

            // return message
            return msg;
        }

        virtual void handleOtherMessage(MessagePtr msg) = 0;

        // Member
    private:
        std::string mSendGateName;
        ApplyKindFunc mApplyKind;
        GetKind mGetKind;

    protected:
        bool mRunning;OPP
        ::cGate* mSendGate;
        MessageCont mMsgCont;
};

#endif /* SENDAWAITEDRETURNBASE_H_ */
