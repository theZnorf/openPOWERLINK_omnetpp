/*
 * OplkHresTimer.h
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#ifndef OPLKHRESTIMER_H_
#define OPLKHRESTIMER_H_

#include "OplkBase.h"

// forward declarations
class HresTimer;
using HresTimerHandle = unsigned long;

namespace interface
{
    class OplkHresTimer : public OplkBase<HresTimer*>
    {
            // Definitions
        public:
            using TimeType = unsigned long long;
            using ArgumentType = unsigned long;
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
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkHresTimer& getInstance();

            static OPLK::ErrorType modifyTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle, TimeType time, TimerCallback callback,
                    ArgumentType arg, OPLK::BoolType cont);
            static OPLK::ErrorType deleteTimer(InstanceHandle instanceHandle, HresTimerHandle* timerHandle);
    };

} /* namespace interface */

#endif /* OPLKHRESTIMER_H_ */
