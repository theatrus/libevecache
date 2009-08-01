#include "reader.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

namespace EveCache {

    CacheFile::CacheFile(const std::string &filename)  : contents(0), length(0), valid(false),
                                                         filename(filename), pos(0)
    {

    }

    CacheFile::CacheFile(const CacheFile&rhs) : length(rhs.length), valid(rhs.valid), 
                                          filename(rhs.filename), pos(rhs.pos) 
    {
        contents = new unsigned char[length];
        memcpy(contents, rhs.contents, length);
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

    bool CacheFile::atEnd() const
    {
        if (pos >= length)
            return true;
        return false;
    }

    

};
