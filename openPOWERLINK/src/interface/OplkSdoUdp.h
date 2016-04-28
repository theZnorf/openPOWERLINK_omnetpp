/*
 * OplkSdoUdp.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKSDOUDP_H_
#define OPLKSDOUDP_H_

#include <OplkBase.h>

// forward declarations
class SdoUdp;

namespace interface
{

    class OplkSdoUdp : public OplkBase<SdoUdp*>
    {
            // Definitions
        public:
            using ConnectionType = OPLK::tSdoUdpCon;
            using FrameType = OPLK::tPlkFrame;
            using SizeType = UINT32;

            // C-Tor / D-Tor
        private:
            OplkSdoUdp();
        public:
            virtual ~OplkSdoUdp();

            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkSdoUdp & getInstance();

            static ErrorType createSocket(InstanceHandle handle, ConnectionType * connection);
            static ErrorType closeSocket(InstanceHandle handle);
            static ErrorType sendToSocket(InstanceHandle handle, ConnectionType * connection, FrameType * data,
                    SizeType size);
            static void ciricalSection(InstanceHandle handle, OPLK::BoolType enable);
    };

} /* namespace interface */

#endif /* OPLKSDOUDP_H_ */
