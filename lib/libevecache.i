%module evecache

%{
#define SWIG_FILE_WITH_INIT
#include <vector>
#include <string>
#include "evecache/reader.hpp"
#include "evecache/parser.hpp"
#include "evecache/market.hpp"
    %}

%include "std_string.i"
%include "std_vector.i"


namespace EveCache {

    typedef enum {
        EStreamStart = 0x7e,
        ETuple = 0x14, // a tuple of N objects
        ETuple2 = 0x15, // a tuple of N objects, variant 2
        E2Tuple = 0x2c, // a tuple of 2 objects
        E1Tuple = 0x25, // a tuple of 1 objects
        E1Tuple2 = 0x27, // a tuple of 1 objects, variant 2
        E0Tuple = 0x24, // a tuple of 0 objects
        E0Tuple2 = 0x26,
        EMarker = 0x11, // A one byte identifier code
        EIdent = 0x13, // an identifier string
        EString = 0x2, // Another type of string, also ids
        EString2 = 0x2e, // stringtastic
        EString3 = 0xf, // really? another? single character string
        EString4 = 0x10,
        EUnicodeString = 0x12,
        EUnicodeString2 = 0x29,
        EEmptyString = 0x28, // empty
        EInteger = 0x04, // 4 byte, little endian?
        EReal = 0x0a, // floating point, 64 bits, assume matches machine double type
        E0Real = 0x0b,
        E0Integer = 0x08, // int  == 0
        ENeg1Integer = 0x07, // int == -1
        E1Integer = 0x09, // int == 1
        EBoolTrue = 0x1f,
        EBoolFalse = 0x20,
        EByte = 0x6, // i think
        ESizedInt = 0x2f, // when you can't decide ahead of time how long to make the integer...
        EShort = 0x05, // 2 byte
        EDict = 0x16, // N objects, consisting of key object and value object
        EObject = 0x17, // Object type ?
        ENone = 0x01, // Python None type
        ESubstream = 0x2b, // substream - len bytes followed by 0x7e
        ELongLong = 0x3, // 64 bit value?
        ECompressedRow = 0x2a, // the datatype from hell, a RLEish compressed row
        EObject22 = 0x22, // a database header field of some variety
        EObject23 = 0x23, // another datatype containing ECompressedRows/DBRows
        ESharedObj = 0x1b, // shared object reference
    } EStreamCode;


    class CacheFile_Iterator;

/***********************************************************************/

    class  SNode {
    public:
        SNode(EStreamCode t);
        SNode(const SNode&);
        virtual ~SNode();
        virtual std::string repl() const;
        virtual EStreamCode type() const;
        virtual void setType(EStreamCode t);
        virtual void addMember(SNode* node);
        virtual const std::vector<SNode*>& members() const;
        virtual SNode* clone() const;
    };

/***********************************************************************/

    class  SStreamNode : public SNode {
    public:
        SStreamNode();
        SStreamNode(EStreamCode t);
        SStreamNode(const SStreamNode &rhs);
        virtual ~SStreamNode() { }
        virtual std::string repl() const;
        virtual SStreamNode* clone() const;
    protected:

    };

/***********************************************************************/

    class  SDBHeader : public SNode {
    public:
        SDBHeader();
        virtual std::string repl() const;
        virtual SDBHeader* clone() const;
    };


/***********************************************************************/

    class  STuple : public SNode {
    public:
        STuple(unsigned int len);
        STuple(const STuple &rhs);
        virtual ~STuple();
        virtual unsigned int givenLength() const;
        virtual void addMember(SNode* node);
        virtual std::string repl() const;
        virtual STuple* clone() const;
    protected:
        unsigned int _givenLength;
    };

/***********************************************************************/

    class  SDict : public SNode {
    public:
        SDict(unsigned int len);
        SDict(const SDict &);
        virtual ~SDict();
        virtual unsigned int givenLength() const;
        virtual void addMember(SNode* node);
        virtual std::string repl() const;
        virtual SDict* clone() const;
        virtual SNode* getByName(const std::string& target) const;
    protected:
        unsigned int _givenLength;
    };


/***********************************************************************/

    class  SNone : public SNode {
    public:
        SNone();
        virtual std::string repl() const;
        virtual SNone* clone() const;
    };


/***********************************************************************/

    class  SMarker : public SNode {
    public:
        SMarker(unsigned char id);
        virtual unsigned char id() const;
        virtual std::string string() const;
        virtual std::string repl() const;
        virtual SMarker* clone() const;
    protected:
        unsigned char _id;
    };

/***********************************************************************/

    class  SIdent : public SNode {
    public:
        SIdent(const std::string& m);
        virtual std::string name() const;
        virtual std::string repl() const;
        virtual SIdent* clone() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class  SString : public SNode {
    public:
        SString(const std::string& m);
        virtual std::string string() const;
        virtual std::string repl() const;
        virtual SString* clone() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class  SInt : public SNode {
    public:
        SInt(int val);
        virtual int value() const;
        virtual std::string repl() const;
        virtual SInt* clone() const;
    };

/***********************************************************************/

    class  SReal : public SNode {
    public:
        SReal(double val);
        virtual double value() const;
        virtual std::string repl() const;
        virtual SReal* clone() const;
    };


/***********************************************************************/

    class  SLongLong : public SNode {
    public:
        SLongLong(long long val);
        virtual long long value() const;
        virtual std::string repl() const;
        virtual SLongLong* clone() const;
    };

/***********************************************************************/

    class  SObject : public SNode {
    public:
        SObject();
        virtual std::string name() const;
        virtual std::string repl() const;
        virtual SObject* clone() const;
    private:
    };


/***********************************************************************/

    class  SSubstream : public SNode {
    public:
        SSubstream(int len);
        virtual std::string repl() const;
        virtual SSubstream* clone() const;
    };

/***********************************************************************/

    class  SDBRow : public SNode {
    public:
        SDBRow(int magic, const std::vector<unsigned char>& data);
        bool isLast() const;
        void setLast(bool l);
        virtual std::string repl() const;
        virtual SDBRow* clone() const;
    };

/***********************************************************************/

    class  SDBRecords : public SNode {
    public:
        SDBRecords();
        virtual std::string repl() const;
        virtual SDBRecords* clone() const;
    };

/***********************************************************************/

    class  Parser {
    public:
        Parser(CacheFile_Iterator *iter);
        ~Parser();
        void parse();
        std::vector<SNode*> streams() const;
    protected:
        SNode* parseone();
        void parse(SNode* node, int limit);
        SNode* getDBRow();
        int getLen();
        unsigned int shareInit();
        void shareAdd(SNode* obj);
        SNode* shareGet(unsigned int id);
        void shareSkip();
    };


    /**
     * Domain object for a market order
     */
    class  MarketOrder {
    public:

        MarketOrder() {}

        /**
         * Utility
         */

        /**
         * Return this item as a standard CSV file export line
         */
        std::string toCsv() const;

        /**
         * Data setters
         */

        void setPrice(unsigned long long v) { _price = v; }
        void setVolRemaining(double v) { _volRemaining = v; }
        void setRange(unsigned int v) { _range = v; }
        void setOrderID(unsigned long long v) { _orderID = v; }
        void setVolEntered(unsigned int v) { _volEntered = v; }
        void setMinVolume(unsigned int v) { _minVolume = v; }
        void setBid(bool b) { _bid = b; }
        void setIssued(unsigned long long v) { _issued = v; }
        void setDuration(unsigned int v) { _duration = v; }
        void setStationID(unsigned int v) { _stationID = v; }
        void setRegionID(unsigned int v) { _regionID = v; }
        void setSolarSystemID(unsigned int v) { _solarSystemID = v; }
        void setJumps(unsigned int v) { _jumps = v; }
        void setType(unsigned int v) { _type = v; }

        /**
         * Data accessors
         */
        unsigned long long price() const { return _price; }
        double volRemaining() const { return _volRemaining; }
        unsigned int range() const { return _range; }
        unsigned long long orderID() const { return _orderID; }
        unsigned int volEntered() const { return _volEntered; }
        unsigned int minVolume() const { return _minVolume; }
        bool isBid() const  { return _bid; }
        unsigned long long issued() const  { return _issued; }
        unsigned int duration() const  { return _duration; }
        unsigned int stationID() const  { return _stationID; }
        unsigned int regionID() const { return _regionID; }
        unsigned int solarSystemID() const  { return _solarSystemID; }
        unsigned int jumps() const  { return _jumps; }
        unsigned int type() const { return _type; }

    };


    class  MarketList {
    public:
        MarketList(int type, int region);
        MarketList();
        MarketList(const MarketList &rhs) {
            _type = rhs._type;
            _region = rhs._region;
            _sellOrders = rhs._sellOrders;
            _buyOrders = rhs._buyOrders;
        }


        void setType(int v) { _type = v; }
        void setRegion(int r) { _region = r; }
        void setTimestamp(unsigned long t) { _ts = t; }

        int type() const { return _type; }
        int region() const { return _region; }
        unsigned long timestamp() const { return _ts; }

        std::vector<MarketOrder> getSellOrders() const { return _sellOrders; }
        std::vector<MarketOrder> getBuyOrders() const { return _buyOrders; }

        void addOrder(MarketOrder& order);

    };


    class  MarketParser {
    public:
        MarketParser(const SNode *stream);
        ~MarketParser();
        MarketList getList() const;
        void parse();
        bool valid() const;
        MarketParser(const char* fileName);
        MarketParser(const std::string fileName);

    };


    class  CacheFile {
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

    };

    class  CacheFile_Iterator { // This does not adhere to the iterator protocol, yet
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


namespace std {
    %template (MarketOrderVector) vector<EveCache::MarketOrder>;
     %template (UnsignedCharVector) vector<unsigned char>;
 }
