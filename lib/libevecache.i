%module libevecache

namespace EveCache {

    class CacheFile {
    public:
        CacheFile(const std::string &filename);
        CacheFile(std::vector<unsigned char> &buf);
        bool readFile();
        int getLength();
        CacheFile_Iterator end() const;
        CacheFile_Iterator begin() const;
        unsigned char byteAt(int) const;
        void peekAt(unsigned char* data, int at, int len) const;
    };
    class CacheFile_Iterator {
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
    };
 }
