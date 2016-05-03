/*
 * OplkException.h
 *
 *  Created on: Apr 26, 2016
 *      Author: franz
 */

#ifndef OPLKEXCEPTION_H_
#define OPLKEXCEPTION_H_

#include <stdexcept>
#include <string>
#include "oplkinc.h"

namespace interface
{

    class OplkException : public std::runtime_error
    {
            // Definitions
        public:
            using ErrorType = OPLK::ErrorType;

            // C-Tor / D-Tor
        public:
            OplkException(std::string const & msg, ErrorType error);
            virtual ~OplkException();

            // Methods
        public:
            virtual const char* what() const noexcept;
            ErrorType  errorNumber() const;

            // Member
        private:
            ErrorType mError;
            std::string mMsg;
    };

} /* namespace interface */

#endif /* OPLKEXCEPTION_H_ */
