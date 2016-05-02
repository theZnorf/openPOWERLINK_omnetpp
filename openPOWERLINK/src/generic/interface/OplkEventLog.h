/*
 * OplkEventLog.h
 *
 *  Created on: May 2, 2016
 *      Author: franz
 */

#ifndef OPLKEVENTLOG_H_
#define OPLKEVENTLOG_H_

#include <functional>
#include "OplkBase.h"

namespace interface
{
    namespace api
    {
        using EventLogLevel = OPLK::tEventlogLevel;
        using EventLogCategory = OPLK::tEventlogCategory;

        struct EventLogFunctions
        {
                std::function<void(EventLogLevel, EventLogCategory, char*)> printMessage;
        };
    }


    class OplkEventLog : public OplkBase<api::EventLogFunctions>
    {
            // C-Tor / D-Tor
        private:
            OplkEventLog();
        public:
            virtual ~OplkEventLog();


            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkEventLog & getInstance();
    };

} /* namespace interface */

#endif /* OPLKEVENTLOG_H_ */
