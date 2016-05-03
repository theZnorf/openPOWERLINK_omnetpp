/*
 * OplkTarget.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKTARGET_H_
#define OPLKTARGET_H_

#include "OplkBase.h"

// forward declarations
class Target;

namespace interface
{
    class OplkTarget : public OplkBase<Target*>
    {
            // Definitions
        public:
            using MilliSecondsType = UINT32;
            using AddressType = UINT32;
            using MtuType = UINT16;
            using TickType = UINT32;
            using LedType = OPLK::tLedType;

            // C-Tor / D-Tor
        private:
            OplkTarget();
        public:
            virtual ~OplkTarget();

            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkTarget& getInstance();

            static void msleep(InstanceHandle instanceHandle, MilliSecondsType milliseconds);
            static ErrorType setIpAddress(InstanceHandle instanceHandle, char* interfaceName, AddressType ipAddress, AddressType subnetMask, MtuType mtu);
            static ErrorType setDefaultGateway(InstanceHandle instanceHandle, AddressType defaultGateway);
            static TickType getTickCount(InstanceHandle instanceHandle);
            static ErrorType setLed(InstanceHandle instanceHandle, LedType ledType, OPLK::BoolType ledOn);

    };

} /* namespace interface */

#endif /* OPLKTARGET_H_ */
