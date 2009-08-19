#ifndef _EC_DBTYPES_H_
#define _EC_DBTYPES_H_

#include <map>
#include <string>

namespace EveCache {

    enum DbType {
        Int64 = 20,
        Time64 = 64,
        Currency64 = 6,
        Int32 = 3,
        Int16 = 2,
        Int8 = 17,
        Double = 5,
        Bool = 11,
        String = 129,
        String2 = 130,
    };

    unsigned long windows_to_unix_time(unsigned long long);
    
    class ColumnLookup {
    public:
        static unsigned char lookupId(std::string& name);
        static std::string lookupName(unsigned char e);

    private:
        ColumnLookup() {}
        ColumnLookup(const ColumnLookup &rhs) {}
        static bool isInit;
        static void init();
        static void insert(const char*, unsigned char id);
        static std::map<std::string, unsigned char> _idLookup;
        static std::map<unsigned char, std::string> _nameLookup; // fixme, just bad, no need for this
    };
    

};


#endif
