/*
 * OplkProcessSync.h
 *
 *  Created on: Jun 15, 2016
 *      Author: franz
 */

#ifndef OPLKPROCESSSYNC_H_
#define OPLKPROCESSSYNC_H_

#include <OplkBase.h>

// forward declaration
class ProcessSync;

namespace interface
{
    class OplkProcessSync : public OplkBase<ProcessSync*>
    {
            // C-Tor
        private:
            OplkProcessSync();
            // D-Tor
        public:
            virtual ~OplkProcessSync();

            // Methods
            // Methods
        public:
            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) override;

            // Static Methods
        public:
            static OplkProcessSync& getInstance();

            static ErrorType processSyncCb(InstanceHandle handle);

    };

} /* namespace interface */

#endif /* OPLKPROCESSSYNC_H_ */
