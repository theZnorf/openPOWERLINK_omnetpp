/*
 * SharedLibraryHelper.cc
 *
 *  Created on: Apr 25, 2016
 *      Author: franz
 */

#include "SharedLibraryHelper.h"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace interface;


#if defined(__linux__)
// Linux types
using UnknownStruct = struct unknown_struct {
   void*  pointers[3];
   struct unknown_struct* ptr;
};
using LinkMap = struct link_map;
#elif defined(_WIN32)
// Windows types
#endif

SharedLibraryHelper::SharedLibraryHelper(const std::string& libname)
    : SharedLibraryHelper(libname, 0)
{}

SharedLibraryHelper::SharedLibraryHelper(const std::string& libname, InstanceType numberOfParallelInstances)
    : SharedLibraryHelper(libname, numberOfParallelInstances, 0)
{}


SharedLibraryHelper::SharedLibraryHelper(std::string const & libname, InstanceType numberOfParallelInstances, InstanceType instanceId)
    : cLibName(libname), mInstanceId(instanceId), cMaxInstanceId(numberOfParallelInstances -1)
{
    createLibraryInstance();

    // open shared library
    mLibHandle = SharedLibraryHelper::openSharedLibrary(getLibraryName());
}

SharedLibraryHelper::~SharedLibraryHelper()
{
    try
    {
        // close shared library
        SharedLibraryHelper::closeShareLibrary(mLibHandle);

        // check if the library was a created copy
        if (cMaxInstanceId > 0)
        {
            auto name = getLibraryName();
            // check if library is not loaded anymore (all other instances are already destroyed)
            if (!SharedLibraryHelper::isSharedLibraryLoaded(name))
            {
                std::cout << "Remove library " << name << std::endl;
                std::remove(name.c_str());
            }
        }
    }
    catch (exception const & e)
    {
    }
}

SharedLibraryHelper::HelperPtr SharedLibraryHelper::getNextLibrary()
{
    if (mInstanceId + 1 <= cMaxInstanceId)
    {
        HelperPtr helper(new SharedLibraryHelper(cLibName, cMaxInstanceId +1, mInstanceId + 1));
        return helper;
    }
    throw runtime_error("SharedLibraryHelper::getNextLibrary - maximum number of instances reached");
}

std::string SharedLibraryHelper::getLibraryName()
{
    return cLibName + ((cMaxInstanceId > 0)? to_string(mInstanceId) : "") + getExtension();
}

void SharedLibraryHelper::createLibraryInstance()
{
    // check if multiple instances are used
    if (cMaxInstanceId > 0)
    {
        ifstream in(cLibName + getExtension(), ios::binary);
        if (!in)
            throw runtime_error("error copying library " + cLibName + getExtension() + " with errno " + to_string(errno));

        auto copyName = getLibraryName();
        // check if copy already exisist
        {
            ifstream check(copyName);
            if (check.good())
                return;
        }
        ofstream out(copyName, ios::binary);
        if (!out)
            throw runtime_error("error opening copied library " + copyName);

        out << in.rdbuf();
    }
}

SharedLibraryHelper::HelperPtr SharedLibraryHelper::createInstance(const std::string& libname)
{
    return SharedLibraryHelper::createInstance(libname, 0);
}

SharedLibraryHelper::HelperPtr SharedLibraryHelper::createInstance(const std::string& libname,
        InstanceType numberOfParallelInstances)
{
    HelperPtr helper(new SharedLibraryHelper(libname, numberOfParallelInstances));
    return helper;
}

LibraryHandle SharedLibraryHelper::openSharedLibrary(const std::string& libname)
{
    // forward to OS specific method
#if defined(__linux__)
    return SharedLibraryHelper::openSharedLibraryLinux(libname);
#elif defined(_WIN32)
    return SharedLibraryHelper::openSharedLibraryWindows(libName);
#endif
}

void SharedLibraryHelper::closeShareLibrary(LibraryHandle handle)
{
    // forward to OS specific method
#if defined(__linux__)
    SharedLibraryHelper::closeShareLibraryLinux(handle);
#elif defined(_WIN32)
    SharedLibraryHelper::closeShareLibraryWindows(handle);
#endif
}

std::string interface::SharedLibraryHelper::getError()
{
    // forward to OS specific method
#if defined(__linux__)
    return SharedLibraryHelper::getErrorLinux();
#elif defined(_WIN32)
    return SharedLibraryHelper::getErrorWindows();
#endif
}

LibraryHandle SharedLibraryHelper::openSharedLibraryLinux(const std::string& libname)
{
#if defined(__linux__)
    auto handle = dlopen(libname.c_str(), RTLD_NOW);
    if (handle == NULL)
        throw runtime_error("SharedLibraryHelper::openSharedLibraryLinux - error loading library " + libname + " with error " + getError());
    return handle;
#else
    throw runtime_error("error linux function called under different OS");
#endif
}

LibraryHandle SharedLibraryHelper::openSharedLibraryWindows(const std::string& libname)
{
#if defined(_WIN32)
    //TODO implement Windows version
    return nullptr;
#else
    throw runtime_error("error windows function called under different OS");
#endif
}

void SharedLibraryHelper::closeShareLibraryLinux(LibraryHandle handle)
{
#if defined(__linux__)
    dlclose(handle);
#else
    throw runtime_error("error linux function called under different OS");
#endif
}

void SharedLibraryHelper::closeShareLibraryWindows(LibraryHandle handle)
{
#if defined(_WIN32)
    //TODO implement Windows version
#else
    throw runtime_error("error windows function called under different OS");
#endif
}

std::string interface::SharedLibraryHelper::getErrorLinux()
{
#if defined(__linux__)
    return dlerror();
#else
    throw runtime_error("error linux function called under different OS");
#endif
}


std::string SharedLibraryHelper::getErrorWindows()
{
#if defined(_WIN32)
    //TODO implement Windows version
#else
    throw runtime_error("error windows function called under different OS");
#endif
}

std::string SharedLibraryHelper::getExtension()
{
#if defined(__linux__)
    return ".so";
#elif defined(_WIN32)
    return ".dll";
#endif
}

bool interface::SharedLibraryHelper::isSharedLibraryLoaded(const std::string& libname)
{
#if defined(__linux__)
    return SharedLibraryHelper::isSharedLibraryLoadedLinux(libname);
#elif defined(_WIN32)
    return SharedLibraryHelper::isSharedLibraryLoadedWindows(libname);
#endif
}

bool interface::SharedLibraryHelper::isSharedLibraryLoadedLinux(const std::string& libname)
{
#if defined(__linux__)
    auto* handle = dlopen(NULL, RTLD_NOW);
    auto* p = reinterpret_cast<UnknownStruct*>(handle)->ptr;
    auto* map = reinterpret_cast<LinkMap*>(p->ptr);

    for (; map; map = map->l_next)
    {
        if (libname.compare(map->l_name) == 0)
            return true;
    }

    return false;

#else
    throw runtime_error("error linux function called under different OS");
#endif
}

bool interface::SharedLibraryHelper::isSharedLibraryLoadedWindows(const std::string& libname)
{
#if defined(_WIN32)
    //TODO implement Windows version
#else
    throw runtime_error("error windows function called under different OS");
#endif
}
