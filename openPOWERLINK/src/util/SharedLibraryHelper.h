/**
 ********************************************************************************
 \file   SharedLibrary.h

 \brief  Include file for operating system specific calls for shared library
 access

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Franz Profelt (franz.profelt@gmail.com)
 ------------------------------------------------------------------------------*/

#ifndef _INC_shared_library_H_
#define _INC_shared_library_H_

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include <fstream>
#include <string>
#include <functional>
#include <memory>

#if defined(__linux__)
#include <dlfcn.h>
#include <link.h>
#elif defined(_WIN32)
#include <Windows.h>
#include <Winbase.h>
#endif

//------------------------------------------------------------------------------
// const defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// typedef
//------------------------------------------------------------------------------

namespace interface
{

#if defined(__linux__)
    using LibraryHandle = void*;
#elif defined(_WIN32)
    using LibraryHandle = HMODULE;
#endif

    /**
     * \brief Helper class for handling shared libraries of Linux and Windows OS
     *
     * This class provides either static method for loading, unloading of
     * shared libraries and resolving of functions.
     * Using an instance the RAII principle is rrealized with loading the given
     * shared library during creation and unloading it during destruction.
     *
     * Multiple versions of the same shared library can be created as used due
     * to internal copies of thte shared library file.
     */
    class SharedLibraryHelper
    {
            // Definitions
        public:
            using InstanceHandle = unsigned int;
            using InstanceType = int;
            using HelperPtr = std::shared_ptr<SharedLibraryHelper>;

            // C-Tor / D-Tor
        public:
            SharedLibraryHelper(SharedLibraryHelper const &) = delete;
            SharedLibraryHelper(SharedLibraryHelper &&) = default;

        private:
            /**
             * \brief Constructor with given library name.
             *
             * \param libname   Name of the shared library without extension
             */
            explicit SharedLibraryHelper(std::string const & libname);

            /**
             * \brief Constructor with given library name and a maximum number
             * of allowed instances.
             *
             * \param libname                     Name of the shared library
             *                                    without extension
             * \param numberOfParallelInstances   Number of maxmimum creatable
             *                                    instances
             */
            explicit SharedLibraryHelper(std::string const & libname,
                    InstanceType numberOfParallelInstances);

            /**
             * \brief Constructor with given library name, a maximum number of
             * allowed instances and the current instance number.
             *
             * \param libname                     Name of the shared library
             *                                    without extension
             * \param numberOfParallelInstances   Number of maxmimum creatable
             *                                    instances
             * \param instanceId                  Number of the created instance
             */
            explicit SharedLibraryHelper(std::string const & libname,
                    InstanceType numberOfParallelInstances,
                    InstanceType instanceId);

        public:
            ~SharedLibraryHelper();

            // Methods
        public:
            /**
             * \brief Creates and returns the next instance of the stored shared
             * library
             *
             * \return Returns a std::shared_ptr<SharedLibraryHelper> of the
             * newly created instance
             */
            HelperPtr getNextLibrary();

            /**
             * \brief Creates and returns the next instance with the given
             * shared library name
             *
             * \param libraryName   Name of the shared library
             * \return Returns a std::shared_ptr<SharedLibraryHelper> of the
             * newly created instance
             */
            HelperPtr getNextLibrary(std::string const & libraryName);

            /**
             * \brief Resolve function with given function name and according
             * template types from saved shared library
             *
             * This method calls the static resolve function method passing the
             * saved library handle.
             *
             * \param functionName      Name of the function which should be
             * resolved
             * \return std::function object with the according template
             * parameter, or nullptr when resolving fails
             * \tparam TRet Returntype of resolved function
             * \tparam TArgs Variadic types of function arguments
             */
            template<typename TRet, typename ... TArgs>
            std::function<TRet(TArgs...)> getFunction(
                    std::string const & functionName)
            {
                return SharedLibraryHelper::resolveFunction<TRet, TArgs...>(
                        mLibHandle, functionName);
            }

            // Helper Methods
        private:
            std::string getLibraryName();
            void createLibraryInstance();

            // Static Methods
        public:
            /**
             * \brief Create an instance of SharedLibraryHelper with given
             * library name.
             *
             * \param libname   Name of the shared library
             * \return Returns a std::shared_ptr<SharedLibraryHelper> of the
             * newly created instance
             */
            static HelperPtr createInstance(std::string const & libname);

            /**
             * \brief Create an instance of SharedLibraryHelper with given
             * library name and maximum number of allowed instances.
             *
             * \param libname                       Name of the shared library
             * \param numberOfParallelInstances     Maximum number of allowed
             * instances
             * \return Returns a std::shared_ptr<SharedLibraryHelper> of the
             * newly created instance
             */
            static HelperPtr createInstance(std::string const & libname,
                    InstanceType numberOfParallelInstances);

            /**
             * \brief Load given shared library into memory.
             *
             * This method is platform independent and calls the according
             * dependent method
             *
             * \param libname Name of the shared Library
             * \return Handle of the loaded library
             * \throws Throws a std::runtime_error when an error occurs during
             * loading
             */
            static LibraryHandle openSharedLibrary(std::string const & libname);

            /**
             * \brief Resolve function with given function name and according
             * template types
             * from given shared library
             *
             * This method is platform independent and calls the according
             * dependent method
             *
             * \param handle            Handle of the shared Library
             * \param functionName      Name of the function to resolve
             * \return std::function object with the according template
             * parameter, or nullptr when resolving fails
             * \tparam TRet Returntype of resolved function
             * \tparam TArgs Variadic types of function arguments
             */
            template<typename TRet, typename ... TArgs>
            static std::function<TRet(TArgs...)> resolveFunction(
                    LibraryHandle handle, std::string const & functionName)
            {
                // forward to OS specific method
#if defined(__linux__)
                return SharedLibraryHelper::resolveFunctionLinux<TRet, TArgs...>(
                        handle, functionName);
#elif defined(_WIN32)
                return SharedLibraryHelper::resolveFunctionWindows<TRet, TArgs...>(
                        handle, functionName);
#endif
            }

            /**
             * \brief Unload given shared library from memory.
             *
             * This method is platform independent and calls the according
             * dependent method
             *
             * \param handle Handle of the shared library
             * \throws Throws a std::runtime_error when an error occurs during
             * unloading
             */
            static void closeShareLibrary(LibraryHandle handle);

            /**
             * \brief Checks if the shared library with given name is currenty
             * loaded within the application
             *
             * This method is platform independent and calls the according
             * dependent method
             *
             * \param libname Name of the shared library
             * \throws Throws a std::runtime_error when an error occurs during
             * checking
             */
            static bool isSharedLibraryLoaded(std::string const & libname);

            /**
             * \brief Getter for the recent error description occuring within
             * shared library API
             *
             * This method is platform independent and calls the according
             * dependent method
             *
             * \return Returns a error description std::string
             */
            static std::string getError();

            // Static helper methods
        private:
            static LibraryHandle openSharedLibraryLinux(
                    std::string const & libname);
            static LibraryHandle openSharedLibraryWindows(
                    std::string const & libname);

            template<typename TRet, typename ... TArgs>
            static std::function<TRet(TArgs...)> resolveFunctionLinux(
                    LibraryHandle handle, std::string const & functionName)
            {
                auto cName = functionName.c_str();

                auto rawFunc = dlsym(handle, cName);
                std::function<TRet(TArgs...)> func = reinterpret_cast<TRet (*)(
                        TArgs...)>(rawFunc);

                return func;
            }

            template<typename TRet, typename ... TArgs>
            static std::function<TRet(TArgs...)> resolveFunctionWindows(
                    LibraryHandle handle, std::string const & functionName)
            {
#if defined(_WIN32)
                std::function<TRet(TArgs...)> func;
                // TODO implement windows version
                return func;
#else
                throw std::runtime_error(
                        "error windows function called under different OS");
#endif
            }

            static void closeShareLibraryLinux(LibraryHandle handle);
            static void closeShareLibraryWindows(LibraryHandle handle);

            static bool isSharedLibraryLoadedLinux(std::string const & libname);
            static bool isSharedLibraryLoadedWindows(
                    std::string const & libname);

            static std::string getErrorLinux();
            static std::string getErrorWindows();

            static std::string getExtension();

            // Member
        private:
            std::string const cLibName;
            InstanceType mInstanceId;
            InstanceType const cMaxInstanceId;
            LibraryHandle mLibHandle;
    };
}

#endif
