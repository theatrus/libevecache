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

#ifndef _EC_READER_H_
#define _EC_READER_H_

#include <string>
#include <vector>
#include "evecache/config.hpp"

namespace EveCache {

    class CacheFile_Iterator;

    class EVECACHE_API CacheFile {
    public:
        CacheFile(const std::string &filename);
        CacheFile(const CacheFile&);
        CacheFile(std::vector<unsigned char> &buf);
        ~CacheFile();
        bool readFile();
        int getLength() const;
        CacheFile_Iterator end() const;
        CacheFile_Iterator begin() const;

        unsigned char byteAt(int) const;
        void peekAt(unsigned char* data, int at, int len) const;

    private:
        unsigned char* contents;
        int length;
        bool valid;
        std::string filename;
    };

    class EVECACHE_API CacheFile_Iterator { // This does not adhere to the iterator protocol, yet
    public:
        CacheFile_Iterator(CacheFile const*, int position, int valid_length);
        CacheFile_Iterator(const CacheFile_Iterator&);
        ~CacheFile_Iterator();

        CacheFile_Iterator& operator=(const CacheFile_Iterator& other);
        bool operator==(const CacheFile_Iterator& rhs) const;
        bool operator!=(const CacheFile_Iterator& rhs) const;

        bool atEnd() const;
        int position() const;
        int limit() const;
        //CacheFile_Iterator operator+(CacheFile_Iterator& lhs, const int len) const;
        CacheFile_Iterator& operator+=(int len);


        int peekShort() const;
        int peekInt() const;
        unsigned char peekChar() const;
        float peekFloat() const;
        double peekDouble() const;
        std::string peekString(int len) const;



        int readShort();
        int readInt();
        unsigned char readChar();
        float readFloat();
        double readDouble();
        std::string readString(int len);
        long long readLongLong();

        void seek(int pos);
        bool advance(int len);
        void setLimit(int limit);




    private:
        CacheFile const *cacheFile;
        int lastPeek;
        int pos;
        int _limit;
        
    };
};


#endif
