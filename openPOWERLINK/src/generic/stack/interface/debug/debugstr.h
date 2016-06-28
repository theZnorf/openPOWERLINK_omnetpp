/**
********************************************************************************
\file   oplk/debugstr.h

\brief  Definitions for debug-string module

This file contains the definitions for the debug-string module.

*******************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2014, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#ifndef _INC_oplk_debugstr_H_
#define _INC_oplk_debugstr_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "ApiDef.h"

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function prototypes
//------------------------------------------------------------------------------

namespace interface
{
    namespace debug
    {
        const char* getNmtEventStr(interface::api::NmtEvent nmtEvent);
        const char* getEventTypeStr(interface::api::EventType eventType);
        const char* getEventSourceStr(interface::api::EventSource eventSrc);
        const char* getEventSinkStr(interface::api::EventSink eventSink);
        const char* getNmtStateStr(interface::api::NmtState nmtState);
        const char* getApiEventStr(interface::api::ApiEventType ApiEvent);
        const char* getNmtNodeEventTypeStr(interface::api::NmtNodeEvent NodeEventType);
        const char* getNmtBootEventTypeStr(interface::api::NmtBootEvent BootEventType);
        const char* getNmtNodeCommandTypeStr(interface::api::NmtNodeCommand nodeCommand);
        const char* getSdoComConStateStr(interface::api::SdoComConState SdoComConState);
        const char* getRetValStr(interface::api::ErrorType OplkError);
        const char* getEmergErrCodeStr(UINT16 EmergErrCode_p);
        const char* getAbortCodeStr(UINT32 abortCode_p);
        const char* getMessageTypeStr(UINT8 messageType);
    }
}

#endif // _INC_oplk_H_

