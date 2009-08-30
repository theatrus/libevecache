// libevecache - EVE Cache File Reader Library
// Copyright (C) 2009  StackFoundry LLC and Yann Ramin
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
// http://www.stackfoundry.com/libevecache
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
        EUnicodeString = 0x12,
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
        EDBHeader = 0x22, // a database header field of some variety
        ECompressedRow = 0x2a, // the datatype from hell, a RLEish compressed row
        EDBRecords = 0x23, // another datatype containing ECompressedRows/DBRows
        ESharedObj = 0x1b, // shared object reference
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

    protected:

    };

/***********************************************************************/

    class EVECACHE_API SDBHeader : public SNode {
    public:
        SDBHeader();
        virtual std::string repl() const;
    };


/***********************************************************************/

    class EVECACHE_API STuple : public SNode {
    public:
        STuple(unsigned int len);
        virtual ~STuple();
        virtual unsigned int givenLength() const;
        virtual void addMember(SNode* node);
        virtual std::string repl() const;
    protected:
        unsigned int _givenLength;
    };

/***********************************************************************/

    class EVECACHE_API SDict : public SNode {
    public:
        SDict(unsigned int len);
        virtual ~SDict();
        virtual unsigned int givenLength() const;
        virtual void addMember(SNode* node);
        virtual std::string repl() const;
    protected:
        unsigned int _givenLength;
    };


/***********************************************************************/

    class EVECACHE_API SNone : public SNode {
    public:
        SNone();
        virtual std::string repl() const;
    };


/***********************************************************************/

    class EVECACHE_API SMarker : public SNode {
    public:
        SMarker(unsigned char id);
        virtual unsigned char id() const;
        virtual std::string string() const;
        virtual std::string repl() const;
    protected:
        unsigned char _id;
    };

/***********************************************************************/

    class EVECACHE_API SIdent : public SNode {
    public:
        SIdent(const std::string& m);
        virtual std::string name() const;
        virtual std::string repl() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class EVECACHE_API SString : public SNode {
    public:
        SString(const std::string& m);
        virtual std::string name() const;
        virtual std::string repl() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class EVECACHE_API SInt : public SNode {
    public:
        SInt(int val);
        virtual int value() const;
        virtual std::string repl() const;
    private:
        int _value;
    };

/***********************************************************************/

    class EVECACHE_API SReal : public SNode {
    public:
        SReal(double val);
        virtual double value() const;
        virtual std::string repl() const;
    private:
        double _value;
    };


/***********************************************************************/

    class EVECACHE_API SLongLong : public SNode {
    public:
        SLongLong(long long val);
        virtual long long value() const;
        virtual std::string repl() const;
    private:
        long long _value;
    };

/***********************************************************************/

    class EVECACHE_API SObject : public SNode {
    public:
        SObject();
        virtual std::string repl() const;
    private:
    };


/***********************************************************************/

    class EVECACHE_API SSubstream : public SNode {
    public:
        SSubstream(int len);
        virtual std::string repl() const;
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
    };

/***********************************************************************/

    class EVECACHE_API Parser {
    public:
        Parser::Parser(CacheFile_Iterator *iter);
        ~Parser();
        void parse();
        std::vector<SNode*> streams() const;
    protected:
        void parse(SNode* node, int limit);
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
	unsigned int *_sharemap;    // list of slot mappings
	SNode **_shareobj;          // list of already discovered shareds
    };




};

#endif
