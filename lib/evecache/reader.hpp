#ifndef _EC_READER_H_
#define _EC_READER_H_

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
        void peekAt(unsigned char* data, int at, int len) const;

    private:
        unsigned char* contents;
        int length;
        bool valid;
        std::string filename;
    };

    class CacheFile_Iterator { // This does not adhere to the iterator protocol, yet
    public:
        CacheFile_Iterator(CacheFile const*, int position, int valid_length);
        CacheFile_Iterator(const CacheFile_Iterator&);
        ~CacheFile_Iterator();

        CacheFile_Iterator& operator=(const CacheFile_Iterator& other);
        bool operator==(const CacheFile_Iterator& rhs) const;
        bool operator!=(const CacheFile_Iterator& rhs) const;

        bool atEnd() const;

        //CacheFile_Iterator operator+(CacheFile_Iterator& lhs, const int len) const;
        CacheFile_Iterator& operator+=(int len);


        int peekShort() const;
        int peekInt() const;
        char peekChar() const;
        float peekFloat() const;
        double peekDouble() const;
        std::string peekString(int len) const;


        int readShort();
        int readInt();
        char readChar();
        float readFloat();
        double readDouble();
        std::string readString(int len);


        bool advance(int len);



    private:
        CacheFile const *cacheFile;
        int lastPeek;
        int pos;
        int limit;
        
    };
};


#endif
