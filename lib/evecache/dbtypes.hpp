// libevecache - EVE Cache File Reader Library
// Copyright (C) 2009-2010  StackFoundry LLC and Yann Ramin
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://dev.eve-central.com/libevecache/
// http://gitorious.org/libevecache


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
