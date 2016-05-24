/**
 ********************************************************************************
 \file   SharedLibrary.h

 \brief  Include file for operating system specific calls for shared library
 access

 *******************************************************************************/

/*------------------------------------------------------------------------------
 Copyright (c) 2016, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the copyright holders nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
            SharedLibraryHelper(std::string const & libname);
            SharedLibraryHelper(std::string const & libname, InstanceType numberOfParallelInstances);
            SharedLibraryHelper(std::string const & libname, InstanceType numberOfParallelInstances, InstanceType instanceId);

        public:
            ~SharedLibraryHelper();

            // Methods
        public:
            HelperPtr getNextLibrary();

            template<typename TRet, typename ... TArgs>
            std::function<TRet(TArgs...)> getFunction(std::string const & functionName)
            {
                return SharedLibraryHelper::resolveFunction<TRet, TArgs...>(mLibHandle, functionName);
            }

            // Helper Methods
        private:
            std::string getLibraryName();
            void createLibraryInstance();

            // Static Methods
        public:
            static HelperPtr createInstance(std::string const & libname);
            static HelperPtr createInstance(std::string const & libname, InstanceType numberOfParallelInstances);

            static LibraryHandle openSharedLibrary(std::string const & libname);

            template<typename TRet, typename ... TArgs>
            static std::function<TRet(TArgs...)> resolveFunction(LibraryHandle handle, std::string const & functionName)
            {
                // forward to OS specific method
#if defined(__linux__)
                return SharedLibraryHelper::resolveFunctionLinux<TRet, TArgs...>(handle, functionName);
#elif defined(_WIN32)
                return SharedLibraryHelper::resolveFunctionWindows<TRet, TArgs...>(handle, functionName);
#endif
            }

            static void closeShareLibrary(LibraryHandle handle);

            static bool isSharedLibraryLoaded(std::string const & libname);

            static std::string getError();

            // Static helper methods
        private:
            static LibraryHandle openSharedLibraryLinux(std::string const & libname);
            static LibraryHandle openSharedLibraryWindows(std::string const & libname);

            template<typename TRet, typename ... TArgs>
            static std::function<TRet(TArgs...)> resolveFunctionLinux(LibraryHandle handle,
                    std::string const & functionName)
            {
                auto cName = functionName.c_str();

                auto rawFunc = dlsym(handle, cName);
                if (rawFunc == nullptr)
                    throw std::runtime_error("resolveFunctionLinux - error resolving function " + functionName + " with error " + getError());
                std::function<TRet(TArgs...)> func = reinterpret_cast<TRet (*)(TArgs...)>(rawFunc);
                if (func == nullptr)
                    throw std::runtime_error("getFunctionLinux - error casting function " + functionName);

                return func;
            }

            template<typename TRet, typename ... TArgs>
            static std::function<TRet(TArgs...)> resolveFunctionWindows(LibraryHandle handle,
                    std::string const & functionName)
            {
#if defined(_WIN32)
                std::function<TRet(TArgs...)> func;
                // TODO implement windows version
                return func;
#else
                throw std::runtime_error("error windows function called under different OS");
#endif
            }

            static void closeShareLibraryLinux(LibraryHandle handle);
            static void closeShareLibraryWindows(LibraryHandle handle);


            static bool isSharedLibraryLoadedLinux(std::string const & libname);
            static bool isSharedLibraryLoadedWindows(std::string const & libname);

            static std::string getErrorLinux();
            static std::string getErrorWindows();

            static std::string getExtension();

            // Member
        private:
            std::string const & cLibName;
            InstanceType mInstanceId;
            InstanceType const cMaxInstanceId;
            LibraryHandle mLibHandle;
    };
}

#endif
