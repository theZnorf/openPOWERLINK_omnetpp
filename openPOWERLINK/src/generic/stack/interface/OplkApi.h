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

namespace interface
{
    class OplkApi : public OplkBase<api::ApiFunctions*>
    {
            // Definitions
        public:

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
    };

} /* namespace interface */

#endif /* OPLKAPI_H_ */
