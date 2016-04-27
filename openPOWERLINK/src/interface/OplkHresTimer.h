/*
 * OplkHresTimer.h
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#ifndef OPLKHRESTIMER_H_
#define OPLKHRESTIMER_H_

#include <vector>
#include "oplkinc.h"
#include <cnedvalue.h>
#include "SharedLibraryHelper.h"

// forward declarations
class HresTimer;
using HresTimerHandle = unsigned long;

namespace interface
{
    class OplkHresTimer
    {
            // Definitions
        public:
            using HresTimerPtr = HresTimer*;

            struct HresTimerInfo
            {
                    HresTimerPtr hresTimer;
                    SharedLibraryHelper::HelperPtr helper;
            };

            using InfoCont = std::vector<HresTimerInfo>;

            using InstanceHandle = SharedLibraryHelper::InstanceHandle;
            using TimeType = unsigned long long;
            using ArgumentType = unsigned long;
            using ErrorType = OPLK::ErrorType;
            using BoolType = unsigned char;
            using TimerEventArgs = OPLK::tTimerEventArg;

            using TimerCallback = ErrorType(TimerEventArgs*);

            // C-Tor / D-Tor
        private:
            OplkHresTimer();
        public:
            virtual ~OplkHresTimer();

            // Methods
        public:
            void initHresTimer(HresTimerPtr hresTimer);
            HresTimerPtr getHresTimer(InstanceHandle handle) const;

            // Member
        private:
            InfoCont mHresTimerInfos;

            // Static Methods
        public:
            static OplkHresTimer& getInstance();

            static OPLK::ErrorType modifyTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle, TimeType time, TimerCallback callback,
                    ArgumentType arg, OPLK::BoolType cont);
            static OPLK::ErrorType deleteTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle);
    };

} /* namespace interface */

#endif /* OPLKHRESTIMER_H_ */
