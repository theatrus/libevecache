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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://dev.eve-central.com/libevecache/
// http://gitorious.org/libevecache

#ifndef _EC_PARSER_H_
#define _EC_PARSER_H_

#include <vector>
#include <string>

#include "evecache/config.hpp"
#include "evecache/dbtypes.hpp"

namespace EveCache {

    typedef enum {
        EStreamStart = 0x7e,
        ENone = 0x01, // Python None type
        EString = 0x2, // Another type of string, also ids
        ELongLong = 0x3, // 64 bit value?
        EInteger = 0x04, // 4 byte, little endian?
        EShort = 0x05, // 2 byte
        EByte = 0x6, // i think
        ENeg1Integer = 0x07, // int == -1
        E0Integer = 0x08, // int  == 0
        E1Integer = 0x09, // int == 1
        EReal = 0x0a, // floating point, 64 bits, assume matches machine double type
        E0Real = 0x0b,
        E0String = 0xe, // 0 length string
        EString3 = 0xf, // really? another? single character string
        EString4 = 0x10,
        EMarker = 0x11, // A one byte identifier code
        EUnicodeString = 0x12,
        EIdent = 0x13, // an identifier string
        ETuple = 0x14, // a tuple of N objects
        ETuple2 = 0x15, // a tuple of N objects, variant 2
        EDict = 0x16, // N objects, consisting of key object and value object
        EObject = 0x17, // Object type ?
        ESharedObj = 0x1b, // shared object reference
        EChecksum = 0x1c,
        EBoolTrue = 0x1f,
        EBoolFalse = 0x20,
        EObject22 = 0x22, // a database header field of some variety
        EObject23 = 0x23, // another datatype containing ECompressedRows/DBRows
        E0Tuple = 0x24, // a tuple of 0 objects
        E1Tuple = 0x25, // a tuple of 1 objects
        E0Tuple2 = 0x26,
        E1Tuple2 = 0x27, // a tuple of 1 objects, variant 2
        EEmptyString = 0x28, // empty
        EUnicodeString2 = 0x29,
        ECompressedRow = 0x2a, // the datatype from hell, a RLEish compressed row
        ESubstream = 0x2b, // substream - len bytes followed by 0x7e
        E2Tuple = 0x2c, // a tuple of 2 objects
        EString2 = 0x2e, // stringtastic
        ESizedInt = 0x2f, // when you can't decide ahead of time how long to make the integer...

    } EStreamCode;


    class CacheFile_Iterator;

/***********************************************************************/

    class EVECACHE_API SNode {
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
    private:
        EStreamCode _type;
    protected:
        std::vector<SNode*> _members;
    };

/***********************************************************************/

    class EVECACHE_API SStreamNode : public SNode {
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

    class EVECACHE_API SDBHeader : public SNode {
    public:
        SDBHeader();
        virtual std::string repl() const;
        virtual SDBHeader* clone() const;
    };


/***********************************************************************/

    class EVECACHE_API STuple : public SNode {
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

    class EVECACHE_API SDict : public SNode {
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

    class EVECACHE_API SNone : public SNode {
    public:
        SNone();
        virtual std::string repl() const;
        virtual SNone* clone() const;
    };


/***********************************************************************/

    class EVECACHE_API SMarker : public SNode {
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

    class EVECACHE_API SIdent : public SNode {
    public:
        SIdent(const std::string& m);
        virtual std::string name() const;
        virtual std::string repl() const;
        virtual SIdent* clone() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class EVECACHE_API SString : public SNode {
    public:
        SString(const std::string& m);
        virtual std::string string() const;
        virtual std::string repl() const;
        virtual SString* clone() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class EVECACHE_API SInt : public SNode {
    public:
        SInt(int val);
        virtual int value() const;
        virtual std::string repl() const;
        virtual SInt* clone() const;
    private:
        int _value;
    };

/***********************************************************************/

    class EVECACHE_API SReal : public SNode {
    public:
        SReal(double val);
        virtual double value() const;
        virtual std::string repl() const;
        virtual SReal* clone() const;
    private:
        double _value;
    };


/***********************************************************************/

    class EVECACHE_API SLongLong : public SNode {
    public:
        SLongLong(long long val);
        virtual long long value() const;
        virtual std::string repl() const;
        virtual SLongLong* clone() const;
    private:
        long long _value;
    };

/***********************************************************************/

    class EVECACHE_API SObject : public SNode {
    public:
        SObject();
        virtual std::string name() const;
        virtual std::string repl() const;
        virtual SObject* clone() const;
    private:
    };


/***********************************************************************/

    class EVECACHE_API SSubstream : public SNode {
    public:
        SSubstream(int len);
        virtual std::string repl() const;
        virtual SSubstream* clone() const;
    private:
        int _len;
    };

/***********************************************************************/

    class EVECACHE_API SDBRow : public SNode {
    public:
        SDBRow(int magic, const std::vector<unsigned char>& data);
        bool isLast() const;
        void setLast(bool l);
        virtual std::string repl() const;
        virtual SDBRow* clone() const;
    private:
        int _id;
        bool _last;
        std::vector<unsigned char> _data;
    };

/***********************************************************************/

    class EVECACHE_API SDBRecords : public SNode {
    public:
        SDBRecords();
        virtual std::string repl() const;
        virtual SDBRecords* clone() const;
    };

/***********************************************************************/

    class EVECACHE_API Parser {
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
    private:
        std::vector<SNode*> _streams;
        CacheFile_Iterator *_iter;
        unsigned int _sharecount;   // number of shared obj
        unsigned int _sharecursor;  // current index into
        SNode **_shareobj;          // list of already discovered shareds
        unsigned int *_sharemap;    // list of slot mappings

    };




};

#endif
