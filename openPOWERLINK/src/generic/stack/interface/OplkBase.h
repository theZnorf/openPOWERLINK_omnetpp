/*
 * OplkBase.h
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#ifndef OPLKBASE_H_
#define OPLKBASE_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include "SharedLibraryHelper.h"
#include "oplkinc.h"

namespace interface
{
    template<typename TModule>
    class OplkBase
    {
            // Definitions
        public:

            struct ModuleInfo
            {
                    TModule module;
                    SharedLibraryHelper::HelperPtr helper;
            };

            using InfoCont = std::vector<ModuleInfo>;
            using InstanceHandle = SharedLibraryHelper::InstanceHandle;
            using Instance = SharedLibraryHelper::InstanceType;
            using ErrorType = OPLK::ErrorType;

            // C-Tor / D-Tor
        protected:
            OplkBase()
            {
                // check static member
                if (mLibName.empty())
                {
                    mLibName = "../openPOWERLINK_V2/stack/lib/linux/x86_64/liboplkmn-sim_d";
                    mNumberOfInstances = 2;
                }
            }

        public:
            virtual ~OplkBase()
            {
            }

            // Methods
        public:
            void initModule(TModule module)
            {
                if (module == nullptr)
                    throw std::invalid_argument("OplkBase::initModule - invalid module pointer");

                // create info object with correct helper instance
                SharedLibraryHelper::HelperPtr helper;

                if (mModuleInfos.empty())
                    helper = SharedLibraryHelper::createInstance(mLibName, mNumberOfInstances);
                else
                    helper = mModuleInfos.back().helper->getNextLibrary(mLibName);

                // add info object
                ModuleInfo info = { module, helper };
                mModuleInfos.push_back(info);

                // set function pointer of interface
                try
                {
                    setFunctions(helper, mModuleInfos.size() - 1);
                }
                catch (std::exception const & e)
                {
                    // remove last module info
                    mModuleInfos.pop_back();

                    // rethrow exception
                    throw e;
                }
            }

            TModule getModule(InstanceHandle handle)
            {
                if (handle < mModuleInfos.size())
                    return mModuleInfos[handle].module;
                else
                    throw std::out_of_range("OplkBase::getModule - invalid instance handle");
            }

            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle) = 0;

            // static Methods
        public:
            static void setLibraryInfo(std::string const & libName, Instance numberOfInstances)
            {
                mLibName = libName;
                mNumberOfInstances = numberOfInstances;
            }

            // Member
        protected:
            InfoCont mModuleInfos;

            // static member
        private:
            static std::string mLibName;
            static Instance mNumberOfInstances;
    };

    template<typename TModule>
    std::string OplkBase<TModule>::mLibName = "";

    template<typename TModule>
    typename OplkBase<TModule>::Instance OplkBase<TModule>::mNumberOfInstances = 1;

} /* namespace interface */

#endif
