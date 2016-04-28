/*
 * OplkBase.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKBASE_H_
#define OPLKBASE_H_

#include <vector>
#include "SharedLibraryHelper.h"
#include "oplkinc.h"

namespace interface
{
    template<typename TModulePtr>
    class OplkBase
    {
            // Definitions
        public:

            struct ModuleInfo
            {
                    TModulePtr module;
                    SharedLibraryHelper::HelperPtr helper;
            };

            using InfoCont = std::vector<ModuleInfo>;
            using InstanceHandle = SharedLibraryHelper::InstanceHandle;
            using ErrorType = OPLK::ErrorType;

            // C-Tor / D-Tor
        protected:
            OplkBase()
            {
            }

        public:
            virtual ~OplkBase()
            {
            }

            // Methods
        public:
            void initModule(TModulePtr module)
            {
                if (module == nullptr)
                    throw std::invalid_argument("OplkBase::initModule - invalid module pointer");

                // create info object with correct helper instance
                SharedLibraryHelper::HelperPtr helper;

                if (mModuleInfos.empty())
                    helper = SharedLibraryHelper::createInstance(/*cLibName*/"lib", /*cNumberOfInstances*/1);
                else
                    helper = mModuleInfos.back().helper->getNextLibrary();

                // add info object
                ModuleInfo info =
                {
                        module,
                        helper
                };
                mModuleInfos.push_back(info);

                // set function pointer of interface
                try
                {
                    setFunctions(helper, mModuleInfos.size() -1);
                }
                catch (std::exception const & e)
                {
                    // remove last module info
                    mModuleInfos.pop_back();

                    // rethrow exception
                    throw e;
                }
            }

            TModulePtr getModule(InstanceHandle handle)
            {
                if (handle < mModuleInfos.size())
                    return mModuleInfos[handle].module;
                else
                    throw std::out_of_range("OplkBase::getModule - invalid instance handle");
            }

            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) = 0;

            // Member
        protected:
            InfoCont mModuleInfos;
    };

} /* namespace interface */

#endif /* OPLKBASE_H_ */
