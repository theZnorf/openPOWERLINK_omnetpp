/*
 * OplkApi.cc
 *
 *  Created on: Apr 28, 2016
 *      Author: franz
 */

#include <OplkApi.h>

namespace interface
{

    OplkApi::OplkApi()
    {
        // TODO Auto-generated constructor stub

    }

    OplkApi::~OplkApi()
    {
        // TODO Auto-generated destructor stub
    }

    OplkApi& OplkApi::getInstance()
    {
        static OplkApi instance;
        return instance;
    }

} /* namespace interface */

void interface::OplkApi::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
{
    // set functions to api
    auto functions = getModule(handle);

    functions->initialize = helper->getFunction("oplk_initialize");
}
