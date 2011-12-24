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

#include "evecache/reader.hpp"
#include "evecache/exceptions.hpp"

#include <fstream>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>

namespace EveCache {


#ifdef WIN32
    CacheFile::CacheFile(const std::wstring &filename)  : contents(0), length(0), valid(false),
                                                         wfilename(filename)
    {

    }

#else
    CacheFile::CacheFile(const std::string &filename)  : contents(0), length(0), valid(false),
                                                         filename(filename)
    {

    }

#endif

    CacheFile::CacheFile(std::vector<unsigned char> &buf)
    {
        length = (int)buf.size()+16; // TODO: unugly padding
        contents = new unsigned char[length];
        unsigned char *cur = contents;
        std::vector<unsigned char>::iterator i = buf.begin();
        for (; i != buf.end(); ++i)
        {
            *cur = *i;
            cur++;
        }
        while (cur < contents+length) { // TODO ... need "virtual" 0 at end of buffer?
            *cur = 0;
            cur++;
        }

        valid = true;
    }

    CacheFile::CacheFile(const CacheFile&rhs) : length(rhs.length), valid(rhs.valid),
												filename(rhs.filename), wfilename(rhs.wfilename)
    {
        contents = new unsigned char[length];
        memcpy(contents, rhs.contents, length);
    }

    CacheFile::~CacheFile()
    {
        if (contents != NULL)
            delete [] contents;
    }

    bool CacheFile::readFile()
    {
        using namespace std;
		#ifdef WIN32
        ifstream file(wfilename, ios::in | ios::binary | ios::ate);
		#endif
        ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);
        if (file.is_open())
        {
            ifstream::pos_type size;
            size = file.tellg();
            contents = new unsigned char [(int)size];
            file.seekg(0, ios::beg);
            file.read(reinterpret_cast<char*>(contents), size);
            file.close();
            valid = true;
            length = static_cast<int>(size);
        }
        return valid;
    }

    int CacheFile::getLength() const
    {
        if (!valid)
            return -1;
        return length;
    }

    CacheFile_Iterator CacheFile::begin() const
    {
        return CacheFile_Iterator(this, 0, getLength());
    }

    CacheFile_Iterator CacheFile::end() const
    {
        return CacheFile_Iterator(this, length, getLength());
    }

    unsigned char CacheFile::byteAt(int pos) const
    {
        if (pos >= 0 && pos < length)
            return contents[pos];
        throw EndOfFileException();
    }

    void CacheFile::peekAt(unsigned char *data, int at, int len) const
    {
        // Broken for big endian...
        memcpy(data, &contents[at], len);
    }


/**
 * Iterator
 */

    CacheFile_Iterator::CacheFile_Iterator(CacheFile const* cf, int position, int valid_length) :
        cacheFile(cf), lastPeek(0), pos(position), _limit(valid_length)
    {
    }

    CacheFile_Iterator::CacheFile_Iterator(const CacheFile_Iterator& rhs)
        : cacheFile(rhs.cacheFile), lastPeek(rhs.lastPeek), pos(rhs.pos), _limit(rhs._limit)
    {
    }

    CacheFile_Iterator::~CacheFile_Iterator()
    {
    }

    CacheFile_Iterator& CacheFile_Iterator::operator=(const CacheFile_Iterator& rhs)
    {
        pos = rhs.pos;
        _limit = rhs._limit;
        cacheFile = rhs.cacheFile;
        return *this;
    }

    bool CacheFile_Iterator::operator==(const CacheFile_Iterator& rhs) const
    {
        if (rhs.pos == pos && rhs.cacheFile == cacheFile)
            return true;
        return false;
    }

    bool CacheFile_Iterator::operator!=(const CacheFile_Iterator& rhs) const
    {
        return !((*this) == rhs);
    }

    bool CacheFile_Iterator::atEnd() const
    {
        if (pos <= _limit)
            return false;
        return true;
    }

    int CacheFile_Iterator::position() const
    {
        return pos;
    }

    int CacheFile_Iterator::limit() const
    {
        return _limit - pos;
    }

    int CacheFile_Iterator::peekInt() const
    {
        int i = cacheFile->byteAt(pos) |
            (cacheFile->byteAt(pos+1) << 8) |
            (cacheFile->byteAt(pos+2) << (8+8)) |
            (cacheFile->byteAt(pos+3) << (8+8+8));
        return i;
    }

    int CacheFile_Iterator::peekShort() const
    {
        int i = cacheFile->byteAt(pos) |
            (cacheFile->byteAt(pos+1) << 8);
        return i;
    }

    unsigned char CacheFile_Iterator::peekChar() const
    {
        return static_cast<unsigned char>(cacheFile->byteAt(pos));
    }

    float CacheFile_Iterator::peekFloat() const
    {
        assert(sizeof(float) == 4);

        float f = 0;
        unsigned char *i = reinterpret_cast<unsigned char*>(&f);
        cacheFile->peekAt(i, pos, sizeof(float));
        return f;
    }

    double CacheFile_Iterator::peekDouble() const
    {
        assert(sizeof(double) == 8);

        double f = 0;
        unsigned char *i = reinterpret_cast<unsigned char*>(&f);
        cacheFile->peekAt(i, pos, sizeof(double));
        return f;

    }

    std::string CacheFile_Iterator::peekString(int len) const
    {
        // TODO: This is dumb
        unsigned char *tmp = new unsigned char[len+1];
        try {
            cacheFile->peekAt(tmp, pos, len);
            tmp[len] = '\0';
            std::string r(reinterpret_cast<char*>(tmp), len);
            delete [] tmp;
            return r;
        } catch (EndOfFileException &e) {
            delete [] tmp;
            throw e;
        }
    }

    void CacheFile_Iterator::seek(int lpos)
    {
        pos = lpos;
    }

    bool CacheFile_Iterator::advance(int len)
    {
        pos += len;
        return atEnd();
    }

    void CacheFile_Iterator::setLimit(int len)
    {
        _limit = pos + len;
    }


    int CacheFile_Iterator::readShort()
    {
        int r = peekShort();
        advance(2);
        return r;
    }

    int CacheFile_Iterator::readInt()
    {
        int r = peekInt();
        advance(4);
        return r;
    }

    unsigned char CacheFile_Iterator::readChar()
    {
        unsigned char r = peekChar();
        advance(1);
        return r;
    }

    float CacheFile_Iterator::readFloat()
    {
        float r = peekFloat();
        advance(4);
        return r;
    }

    double CacheFile_Iterator::readDouble()
    {
        double r = peekDouble();
        advance(8);
        return r;
    }

    std::string CacheFile_Iterator::readString(int len)
    {
        std::string r = peekString(len);
        advance(len);
        return r;
    }

    long long CacheFile_Iterator::readLongLong()
    {
        unsigned int a = static_cast<unsigned int>(readInt());
        unsigned int b = static_cast<unsigned int>(readInt());
        long long c = (long long)a | ((long long)b << 32);
        return c;
    }


    CacheFile_Iterator& CacheFile_Iterator::operator+=(int len)
    {
        advance(len);
        return *this;
    }



};
