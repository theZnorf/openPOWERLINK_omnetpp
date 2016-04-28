/*
 * OplkEdrv.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKEDRV_H_
#define OPLKEDRV_H_

#include <OplkBase.h>

// forward declaration
class Edrv;


namespace interface
{

    class OplkEdrv : public OplkBase<Edrv*>
    {
            // Definitions
        public:
            using EdrvInitParamType = OPLK::tEdrvInitParam;
            using MacType = UINT8*;
            using TxBufferType = OPLK::tEdrvTxBuffer;
            using FilterType = OPLK::tEdrvFilter;

            // C-Tor / D-Tor
        private:
            OplkEdrv();
        public:
            virtual ~OplkEdrv();

            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkEdrv& getInstance();

            static ErrorType initEdrv(InstanceHandle handle, EdrvInitParamType* initParam);
            static ErrorType exitEdrv(InstanceHandle handle);
            static MacType getMacAddr(InstanceHandle handle);
            static ErrorType sendTxBuffer(InstanceHandle handle, TxBufferType* txBuffer);
            static ErrorType allocTxBuffer(InstanceHandle handle, TxBufferType* txBuffer);
            static ErrorType freeTxBuffer(InstanceHandle handle, TxBufferType* txBuffer);
            static ErrorType changeRxFilter(InstanceHandle handle, FilterType* filter, UINT count, UINT entryChanged, UINT changeFlags);
            static ErrorType clearRxMulticastMacAddr(InstanceHandle handle, MacType macAddr);
            static ErrorType setRxMulticastMacAddr(InstanceHandle handle, MacType macAddr);

    };

} /* namespace interface */

#endif /* OPLKEDRV_H_ */
