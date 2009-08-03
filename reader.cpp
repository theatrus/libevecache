#include "reader.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

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
    

/**
 * Iterator 
 */

    CacheFile_Iterator::CacheFile_Iterator(CacheFile const *cf, int position, int valid_length) :
        cacheFile(cf), pos(position), limit(valid_length)
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
        return 0;
    }

    char CacheFile_Iterator::peekChar() const
    {
        return '0';
    }

    float CacheFile_Iterator::peekFloat() const
    {
        return 0.0;
    }

    double CacheFile_Iterator::peekDouble() const
    {
        return 0.0;
    }

    std::string CacheFile_Iterator::peekString(int len) const
    {
        return std::string("");
    }

    bool CacheFile_Iterator::advance(int len)
    {
        pos += len;
        if (len <= limit)
            return true;
        return false;
    }

    CacheFile_Iterator& CacheFile_Iterator::operator+=(int len)
    {
        advance(len);
        return *this;
    }



};
