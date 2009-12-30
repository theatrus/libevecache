// libevecache - EVE Cache File Reader Library
// Copyright (C) 2009-2010  StackFoundry LLC and Yann Ramin
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// http://dev.eve-central.com/libevecache/
// http://gitorious.org/libevecache

#ifndef _EC_EXCEPTIONS_H_
#define _EC_EXCEPTIONS_H_

#include <string>
#include "evecache/config.hpp"

namespace EveCache {

    class EVECACHE_API EndOfFileException {
    public:
        EndOfFileException();
        operator std::string () const;
    };

    class EVECACHE_API ParseException {
    public:
        ParseException(const std::string& msg);
        ParseException(const char* m);
        operator std::string () const;
    private:
        std::string message;
    };


};


#endif
