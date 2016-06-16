/*
 * OplkApiEvent.h
 *
 *  Created on: Jun 15, 2016
 *      Author: franz
 */

#ifndef OPLKAPIEVENT_H_
#define OPLKAPIEVENT_H_

#include <OplkBase.h>
#include "ApiDef.h"

// forward declaration
class ApiEvent;

namespace interface
{
    class OplkApiEvent : public OplkBase<ApiEvent*>
    {
            // Definitions
        public:
            using ApiEventType = api::ApiEventType;
            using ApiEventArg = api::ApiEventArg;
            // C-Tor
        private:
            OplkApiEvent();
            // D-Tor
        public:
            virtual ~OplkApiEvent();

            // Methods
            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkApiEvent& getInstance();

            static ErrorType eventCb(InstanceHandle handle, ApiEventType eventType, ApiEventArg* eventArg, void* userArg);

    };

} /* namespace interface */

#endif /* OPLKAPIEVENT_H_ */
