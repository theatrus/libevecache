#ifndef _EC_READER_H_

#include <string>


namespace EveCache {

    class CacheFile_Iterator;

    class CacheFile {
    public:
        CacheFile(const std::string &filename);
        CacheFile(const CacheFile&);
        ~CacheFile();
        bool readFile();
        int getLength() const;
        CacheFile_Iterator end() const;
        CacheFile_Iterator begin() const;

        unsigned char byteAt(int) const;

    private:
        unsigned char* contents;
        int length;
        bool valid;
        std::string filename;
    };

    class CacheFile_Iterator { // This does not adhere to the iterator protocol, yet
    public:
        CacheFile_Iterator(CacheFile const*, int position, int valid_length);
        ~CacheFile_Iterator();

        CacheFile_Iterator& operator=(const CacheFile_Iterator& other);
        bool operator==(const CacheFile_Iterator& rhs) const;
        bool operator!=(const CacheFile_Iterator& rhs) const;
        
        int peekInt() const;
        char peekChar() const;
        float peekFloat() const;
        double peekDouble() const;
        std::string peekString(int len) const;

        bool advance(int len);
        //CacheFile_Iterator operator+(CacheFile_Iterator& lhs, const int len) const;
        CacheFile_Iterator& operator+=(int len);

    private:
        CacheFile const *cacheFile;
        int pos;
        int limit;
        
    };
};


#endif
