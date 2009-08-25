#include "evecache/dbtypes.hpp"

namespace EveCache {
    unsigned long windows_to_unix_time(unsigned long long time)
    {
        return (time / 10000000LL)-11644473600LL;
    }

    std::map<std::string, unsigned char> ColumnLookup::_idLookup;
    std::map<unsigned char, std::string> ColumnLookup::_nameLookup;
    bool ColumnLookup::isInit = false;

    void ColumnLookup::insert(const char* name, unsigned char id)
    {
        _idLookup[name] = id;
        _nameLookup[id] = name;
    }

    void ColumnLookup::init()
    {
        if (ColumnLookup::isInit == true)
            return;
        insert("bid", 116);
        insert("duration", 126);
        insert("issued", 131);
        insert("market", 136);
        insert("minVolume", 137);
        insert("orderID", 138);
        insert("price", 139);
        insert("range", 140);
        insert("regionID", 141);
        insert("solarSystemID", 150);
        insert("stationID", 155);
        insert("volEntered", 160);
        insert("volRemain", 161);
        insert("jumps", 41);
        insert("typeID", 74);
        ColumnLookup::isInit = true;
    }


    unsigned char ColumnLookup::lookupId(std::string& name)
    {
        init();
        return _idLookup[name];
    }

    std::string ColumnLookup::lookupName(unsigned char id)
    {
        init();
        return _nameLookup[id];
    }

};
