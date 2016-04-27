/*
 * OplkException.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#include <OplkException.h>

namespace interface
{

    OplkException::OplkException(std::string const & msg, ErrorType error)
        : runtime_error(msg), mError(error), mMsg(msg)
    {}

    OplkException::~OplkException()
    {}

    const char* OplkException::what() const noexcept
    {
        return ("OplkException: " + mMsg + " with error number " + std::to_string(mError)).c_str();
    }

    OplkException::ErrorType  OplkException::errorNumber() const
    {
        return mError;
    }

} /* namespace interface */
