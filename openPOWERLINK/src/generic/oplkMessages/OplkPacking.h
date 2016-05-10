/*
 * OplkPacking.h
 *
 *  Created on: May 3, 2016
 *      Author: franz
 */

#ifndef OPLKPACKING_H_
#define OPLKPACKING_H_

#include "ApiDef.h"

namespace oplkMessages
{
    using PointerCont = UINT64;

    // pointer cont
    void doUnpacking(OPP::cCommBuffer *b, PointerCont& cont);
    void doPacking(OPP::cCommBuffer *b, PointerCont& cont);

    // init params
    void doUnpacking(OPP::cCommBuffer *b, interface::api::ApiInitParam& param);
    void doPacking(OPP::cCommBuffer *b, interface::api::ApiInitParam& param);

    // api event args
    void doUnpacking(OPP::cCommBuffer *b, interface::api::ApiEventArg& arg);
    void doPacking(OPP::cCommBuffer *b, interface::api::ApiEventArg& arg);

    // hw param
    void doUnpacking(OPP::cCommBuffer* b, interface::api::HwParam& param);
    void doPacking(OPP::cCommBuffer* b, interface::api::HwParam& param);

    // obd init
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdInitParam& param);
    void doPacking(OPP::cCommBuffer* b, interface::api::ObdInitParam& param);

    // obd entry
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdEntry& entry);
    void doPacking(OPP::cCommBuffer* b, interface::api::ObdEntry& entry);

    // obd subentry
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdSubEntry& subentry);
    void doPacking(OPP::cCommBuffer* b, interface::api::ObdSubEntry& subentry);

    // obd cb param
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdCallbackParam& param);
    void doPacking(OPP::cCommBuffer* b, interface::api::ObdCallbackParam& param);

    // obd al conn hdl
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ObdAlConnHdl& connHdl);
    void doPacking(OPP::cCommBuffer* b, interface::api::ObdAlConnHdl& connHdl);

    // nettime
    void doUnpacking(OPP::cCommBuffer* b, interface::api::NetTime& time);
    void doPacking(OPP::cCommBuffer* b, interface::api::NetTime& time);

    // error history entry
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ErrHistoryEntry& entry);
    void doPacking(OPP::cCommBuffer* b, interface::api::ErrHistoryEntry& entry);

    // async sco com
    void doUnpacking(OPP::cCommBuffer* b, interface::api::AsySdoCom& com);
    void doPacking(OPP::cCommBuffer* b, interface::api::AsySdoCom& com);

    // stack info
    void doUnpacking(OPP::cCommBuffer* b, interface::api::ApiStackInfo& info);
    void doPacking(OPP::cCommBuffer* b, interface::api::ApiStackInfo& info);

    // soc time info
    void doUnpacking(OPP::cCommBuffer* b, interface::api::SocTimeInfo& info);
    void doPacking(OPP::cCommBuffer* b, interface::api::SocTimeInfo& info);

    // templates for iterables
    template<typename TIter, typename TSize>
    void doUnpacking(OPP::cCommBuffer* b, TIter iter, TSize size)
    {
        //TODO: check allocation
        for (TSize i = 0; i < size; i++)
            doUnpacking(b, iter[i]);
    }

    template<typename TIter, typename TSize>
    void doPacking(OPP::cCommBuffer* b, TIter iter, TSize size)
    {
        for (TSize i = 0; i < size; i++)
            doPacking(b, iter[i]);
    }

} /* namespace oplkMessages */

#endif /* OPLKPACKING_H_ */
