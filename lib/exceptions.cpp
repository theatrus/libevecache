#include "evecache/exceptions.hpp"

namespace EveCache {
    ParseException::ParseException(const std::string &m) : message(m)
    {
    }

    ParseException::ParseException(const char*m)
    {
        message = std::string(m);
    }

    ParseException::operator std::string () const
    {
        return message;
    }

};
