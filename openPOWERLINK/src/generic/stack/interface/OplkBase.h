/**
 ********************************************************************************
 \file   OplkBase.h

 \brief  Include file with template base class for all interface clases

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#ifndef OPLKBASE_H_
#define OPLKBASE_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include "SharedLibraryHelper.h"
#include "oplkinc.h"

namespace interface
{
    /**
     * Template base class for each interface module.
     * This class provides handling and assiciation of shared library instances
     * with the according module instances.
     */
    template<typename TModule>
    class OplkBase
    {
            // Definitions
        public:
            /**
             * Structure with module instance and accroding shared library helper
             */
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
            /**
             * \brief Protected default constructor
             */
            OplkBase()
            {
            }

        public:
            virtual ~OplkBase()
            {
            }

            // Methods
        public:
            /**
             * \brief Initialize interface functionality with given module.
             *
             * Initializes the interface functionality saving the given module
             * with a newly created shared library helper
             *
             * \param module    Instance of module which is saved in internal
             * container
             */
            void initModule(TModule module)
            {
                if (module == nullptr)
                    throw std::invalid_argument(
                            "OplkBase::initModule - invalid module pointer");

                // create info object with correct helper instance
                SharedLibraryHelper::HelperPtr helper;

                if (mModuleInfos.empty())
                    helper = SharedLibraryHelper::createInstance(mLibName,
                            mNumberOfInstances);
                else
                    helper = mModuleInfos.back().helper->getNextLibrary(
                            mLibName);

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

            /**
             * \brief Getter for the saved module instance
             *
             * This getter returns the saved module instance identified by the
             * passed handle.
             *
             * \param handle    Handle of requested module
             * \return Returns the saved instance according to the given handle.
             * \throws Throws a std::out_of_range exception when the handle is
             * invalid
             */
            TModule getModule(InstanceHandle handle)
            {
                if (handle < mModuleInfos.size())
                    return mModuleInfos[handle].module;
                else
                    throw std::out_of_range(
                            "OplkBase::getModule - invalid instance handle");
            }

            virtual void setFunctions(SharedLibraryHelper::HelperPtr helper,
                    InstanceHandle handle) = 0;

            // static Methods
        public:
            /**
             * \brief Static setter for libary information
             *
             * Setter for static library informations, which will be passed to
             * the created shared library helper
             */
            static void setLibraryInfo(std::string const & libName,
                    Instance numberOfInstances)
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
