#ifndef _EC_READER_H_

#include <string>


namespace EveCache {

    class CacheFile {
    public:
        CacheFile(const std::string &filename);
        CacheFile(const CacheFile&);
        bool readFile();
        int getLength() const;
        bool atEnd() const;

        unsigned char byteAt(int) const;
        unsigned char peekByte() const;
        unsigned int peekInt() const;

    private:
        unsigned char* contents;
        int length;
        bool valid;
        std::string filename;

        int pos;
    };
};


#endif
