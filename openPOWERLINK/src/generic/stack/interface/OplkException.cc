/*
 * OplkException.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#include <OplkException.h>
#include "debugstr.h"

namespace interface
{

    OplkException::OplkException(std::string const & msg, ErrorType error)
        : runtime_error(msg), mError(error), mMsg(msg)
    {}

    OplkException::~OplkException()
    {}

    const char* OplkException::what() const noexcept
    {
        return debug::getRetValStr(mError);
    }

    OplkException::ErrorType  OplkException::errorNumber() const
    {
        return mError;
    }

} /* namespace interface */
