/*
 * OplkApi.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKAPI_H_
#define OPLKAPI_H_

#include <functional>
#include "OplkBase.h"
#include "ApiDef.h"
#include "OplkException.h"

// forward declarations
class Api;

namespace interface
{
    class OplkApi : public OplkBase<Api*>
    {
            // Definitions
        public:
            using ApiEventType = api::ApiEventType;
            using ApiEventArg = api::ApiEventArg;

            // C-Tor / D-Tor
        private:
            OplkApi();
        public:
            virtual ~OplkApi();

            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkApi & getInstance();

            static ErrorType processSyncCb(InstanceHandle handle);
            static ErrorType eventCb(InstanceHandle handle, ApiEventType eventType, ApiEventArg* eventArg, void* userArg);
    };

} /* namespace interface */

#endif /* OPLKAPI_H_ */
