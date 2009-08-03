#include "evecache/reader.hpp"

#include <fstream>
#include <iostream>
#include <cstring>
#include <assert.h>

namespace EveCache {

    CacheFile::CacheFile(const std::string &filename)  : contents(0), length(0), valid(false),
                                                         filename(filename)
    {

    }

    CacheFile::CacheFile(const CacheFile&rhs) : length(rhs.length), valid(rhs.valid), 
                                          filename(rhs.filename)
    {
        contents = new unsigned char[length];
        memcpy(contents, rhs.contents, length);
    }

    CacheFile::~CacheFile()
    {
        if (contents != 0)
            delete contents;
    }

    bool CacheFile::readFile()
    {
        using namespace std;
        ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);
        if (file.is_open())
        {
            ifstream::pos_type size;
            size = file.tellg();
            contents = new unsigned char [size];
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
        return CacheFile_Iterator(this, length + 1, getLength());
    }
    
    unsigned char CacheFile::byteAt(int pos) const
    {
        if (pos < length)
            return contents[pos];
        return 0xFF;
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
        cacheFile(cf), lastPeek(0), pos(position), limit(valid_length)
    {
    }

    CacheFile_Iterator::CacheFile_Iterator(const CacheFile_Iterator& rhs)
        : cacheFile(rhs.cacheFile), lastPeek(rhs.lastPeek), pos(rhs.pos), limit(rhs.limit)
    {
    }

    CacheFile_Iterator::~CacheFile_Iterator()
    {
    }

    CacheFile_Iterator& CacheFile_Iterator::operator=(const CacheFile_Iterator& rhs)
    {
        pos = rhs.pos;
        limit = rhs.limit;
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

    char CacheFile_Iterator::peekChar() const
    {
        return static_cast<char>(cacheFile->byteAt(pos));
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
        unsigned char *tmp = new unsigned char[len+1];
        cacheFile->peekAt(tmp, pos, len);
        tmp[len] = '\0';
        std::string r(reinterpret_cast<char*>(tmp), len);
        delete tmp;
        return r;
    }

    bool CacheFile_Iterator::advance(int len)
    {
        pos += len;
        if (len <= limit)
            return true;
        return false;
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

    char CacheFile_Iterator::readChar()
    {
        char r = peekChar();
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

    


    CacheFile_Iterator& CacheFile_Iterator::operator+=(int len)
    {
        advance(len);
        return *this;
    }



};
