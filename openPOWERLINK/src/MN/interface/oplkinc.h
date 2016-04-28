/*
 * oplkinc.h
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#ifndef OPLKINC_H_
#define OPLKINC_H_

#define _INC_common_oplkinc_H_
#define OPLK_NAMESPACE

#ifdef OPLK_NAMESPACE
#define OPLK oplk
#define BEGIN_OPLK  namespace oplk {
#define END_OPLK }
#define USING_OPLK using namespace oplk;
#else
#define OPLK
#define BEGIN_OPLK
#define END_OPLK
#define USING_OPLK
#endif

// include for section and target defines/includes
extern "C"
{
#include <common/section.h>
}

BEGIN_OPLK

// includes for type definitions and error types
    extern "C"
    {
#include <oplk/basictypes.h>
#include <oplk/errordefs.h>
    }

    /**
    \brief Timestamp structure

    The following structure defines a timestamp value use to store target specific
    timestamps.
    */
    typedef struct
    {
        UINT32        timeStamp;      ///< The timestamp.
    } tTimestamp;

    // unsiged long pointer
    using ULONG_PTR = ULONG*;

    extern "C"
    {
#include <sim.h>
    }

    using ErrorType = tOplkError;
    using BoolType = BOOL;
    using LongType = LONG;

#undef BOOL
#undef LONG
#undef ASSERT

END_OPLK

#endif /* OPLKINC_H_ */
