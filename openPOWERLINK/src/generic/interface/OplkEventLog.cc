/*
 * OplkEventLog.cc
 *
 *  Created on: May 2, 2016
 *      Author: franz
 */

#include <OplkEventLog.h>

namespace interface
{

    OplkEventLog::OplkEventLog()
    {
        // TODO Auto-generated constructor stub

    }

    OplkEventLog::~OplkEventLog()
    {
        // TODO Auto-generated destructor stub
    }

    void OplkEventLog::setFunctions(SharedLibraryHelper::HelperPtr helper, InstanceHandle handle)
    {
        // set functions to api
        auto functions = getModule(handle);
    }

    OplkEventLog& OplkEventLog::getInstance()
    {
        static OplkEventLog log;

        return log;
    }

} /* namespace interface */
