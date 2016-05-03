/*
 * OplkTrace.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKTRACE_H_
#define OPLKTRACE_H_

#include "OplkBase.h"

// forward declarations
class Trace;

namespace interface
{

    class OplkTrace : public OplkBase<Trace*>
    {
            // C-Tor / D-Tor
        public:
            OplkTrace();
            virtual ~OplkTrace();

            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkTrace getInstance();

            static void trace(InstanceHandle handle, char const * msg);
    };

} /* namespace interface */

#endif /* OPLKTRACE_H_ */
