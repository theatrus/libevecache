#ifndef _EC_EXCEPTIONS_H_
#define _EC_EXCEPTIONS_H_

#include <string>

namespace EveCache {

    class ParseException {
    public:
        ParseException(const std::string& msg);
        ParseException(const char* m);
        operator std::string () const;
    private:
        std::string message;
    };


};


#endif
