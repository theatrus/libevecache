// libevecache - EVE Cache File Reader Library
// Copyright (C) 2009  StackFoundry LLC and Yann Ramin
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// http://www.stackfoundry.com/libevecache
// http://gitorious.org/libevecache


#include "evecache/parser.hpp"
#include "evecache/reader.hpp"
#include "evecache/exceptions.hpp"

#include <assert.h>

#include <iostream>
#include <iomanip>
#include <sstream>

namespace EveCache {


    struct packer_opcap {
        unsigned char tlen : 3;
        bool tzero : 1;
        unsigned char blen : 3;
        bool bzero : 1;
    };

    static void rle_unpack(const unsigned char* in_buf, int in_length, std::vector<unsigned char> &buffer)
    {
        buffer.clear();
        if(in_length == 0)
            return;

        const unsigned char *end = in_buf + in_length;
        while (in_buf < end)
        {

            const packer_opcap opcap = *(reinterpret_cast<const packer_opcap*>(in_buf++));
            if (opcap.tzero) {
                unsigned char count = opcap.tlen + 1;
                for (; count > 0; count--)
                {
                    buffer.push_back(0);
                }
            } else {
                unsigned int count = 8 - opcap.tlen;
                for (; count > 0 && in_buf < end ; count--) {
                    buffer.push_back(*in_buf++);
                }
            }
            if (opcap.bzero) {
                unsigned char count = opcap.blen + 1;
                for (; count > 0; count--)
                {
                    buffer.push_back(0);
                }
            } else {
                unsigned int count = 8 - opcap.blen;
                for (; count > 0 && in_buf < end ; count--) {
                    buffer.push_back(*in_buf++);
                }
            }

        }
    }



    std::string SNode::repl() const
    {
        std::stringstream msg(std::stringstream::out);
        msg << "<SNode type " << std::hex << type() << ">";
        return msg.str();
    }

    SNode::SNode(EStreamCode t) : _type(t)
    {
    }

    SNode::SNode(const SNode& rhs)
    {
        _members = rhs._members;
    }


    SNode::~SNode()
    {
        std::vector<SNode*>::iterator i = _members.begin();
        for (; i != _members.end(); ++i)
        {
            SNode* sn = *i;
            if (sn) {
                delete sn;
            }
        }
        _members.clear();
    }

    EStreamCode SNode::type() const
    {
        return _type;
    }

    void SNode::setType(EStreamCode t)
    {
        _type = t;
    }

    void SNode::addMember(SNode* node)
    {
        _members.push_back(node);
    }

    const std::vector<SNode*>& SNode::members() const
    {
        return _members;
    }


/***********************************************************************/

    SStreamNode::SStreamNode() : SNode(EStreamStart)
    {
    }

    SStreamNode::SStreamNode(EStreamCode t) : SNode(t)
    {
    }

    SStreamNode::SStreamNode(const SStreamNode& rhs) : SNode(rhs)
    {
    }

    std::string SStreamNode::repl() const
    {
        std::stringstream ss;
        ss << " <SStreamNode> ";
        return ss.str();
    }

/***********************************************************************/

    SDBHeader::SDBHeader() : SNode(EDBHeader)
    {
    }

    std::string SDBHeader::repl() const
    {
        std::stringstream ss;
        ss << " <SDBHeader> ";
        return ss.str();
    }


/***********************************************************************/

    STuple::STuple(unsigned int len) : SNode(ETuple), _givenLength(len)
    {
    }

    STuple::~STuple()
    {
    }

    void STuple::addMember(SNode* node)
    {
        assert(_members.size() < _givenLength);
        _members.push_back(node);
    }


    unsigned int STuple::givenLength() const
    {
        return _givenLength;
    }

    std::string STuple::repl() const
    {
        std::stringstream ss;
        ss << " <STuple> ";
        return ss.str();
    }


/***********************************************************************/

    SDict::SDict(unsigned int len) : SNode(EDict), _givenLength(len)
    {
    }

    SDict::~SDict()
    {
    }

    void SDict::addMember(SNode* node)
    {
        assert(_members.size() < _givenLength);
        _members.push_back(node);
    }

    unsigned int SDict::givenLength() const
    {
        return _givenLength;
    }

    std::string SDict::repl() const
    {
        std::stringstream ss;
        ss << " <SDict> ";
        return ss.str();
    }


/***********************************************************************/


    SMarker::SMarker(char i) : SNode(EMarker), _id(i)
    {
    }

    std::string SMarker::repl() const
    {
        std::stringstream ss;
        ss << " <SMarker ID: " << static_cast<int>(id()) << " > ";
        return ss.str();
    }

    char SMarker::id() const
    {
        return _id;
    }

/***********************************************************************/

    SIdent::SIdent(const std::string& n) : SNode(EIdent), _name(n)
    {
    }

    std::string SIdent::repl() const
    {
        std::stringstream ss;
        ss << " <SIdent '" << name() << "'> ";
        return ss.str();
    }

    std::string SIdent::name() const
    {
        return _name;
    }


/***********************************************************************/

    SString::SString(const std::string& n) : SNode(EString), _name(n)
    {
    }

    std::string SString::name() const
    {
        return _name;
    }

    std::string SString::repl() const
    {
        std::stringstream ss;
        ss << " <SString '" << name() << "'> ";
        return ss.str();
    }



/***********************************************************************/

    SInt::SInt(int val) : SNode(EInteger), _value(val)
    {
    }

    int SInt::value() const
    {
        return _value;
    }

    std::string SInt::repl() const
    {
        std::stringstream ss;
        ss << " <SInt '" << value() << "'> ";
        return ss.str();
    }

/***********************************************************************/

    SLongLong::SLongLong(long long val) : SNode(ELongLong), _value(val)
    {
    }

    long long SLongLong::value() const
    {
        return _value;
    }

    std::string SLongLong::repl() const
    {
        std::stringstream ss;
        ss << " <SLongLong '" << value() << "'> ";
        return ss.str();
    }


/***********************************************************************/

    SObject::SObject() : SNode(EObject)
    {
    }

    std::string SObject::repl() const
    {
        std::stringstream ss;
        ss << " <SObject> ";
        return ss.str();
    }



/***********************************************************************/

    SNone::SNone() : SNode(ENone)
    {
    }
    std::string SNone::repl() const
    {
        std::stringstream ss;
        ss << " <NONE> ";
        return ss.str();
    }



/***********************************************************************/

    SSubstream::SSubstream(int len) : SNode(ESubstream), _len(len)
    {
    }

    std::string SSubstream::repl() const
    {
        std::stringstream ss;
        ss << " <SSubstream> ";
        return ss.str();
    }

/***********************************************************************/

    SDBRow::SDBRow(int magic, const std::vector<unsigned char>& data) 
        : SNode(ECompressedRow), _id(magic), _data(data)
    {
    }

    std::string SDBRow::repl() const
    {
        std::stringstream ss;
        ss << " <DBRow ";

        std::vector<unsigned char>::const_iterator kk = _data.begin();

         for (; kk != _data.end(); ++kk)
        {
            ss << std::setw(2) << 
                std::setfill('0') << std::hex << static_cast<int>(*kk);
        }

        // std::cout << "Got new data! oldlen " << std::dec
        //           << len << " len " <<  newdata.size() << std::endl;

         ss << ">";
        return ss.str();
    }


/***********************************************************************/

    SDBRecords::SDBRecords() : SNode(EDBRecords)
    {
    }

    std::string SDBRecords::repl() const
    {
        std::stringstream ss;
        ss << " <DBRecords> ";
        return ss.str();
    }

/***********************************************************************/

    SStreamIdent::SStreamIdent(int magic) 
        : SNode(EStreamIdent), _id(magic)
    {
    }

    std::string SStreamIdent::repl() const
    {
        std::stringstream ss;
        ss << " <SStreamIdent " << _id << "> ";
        return ss.str();
    }



/***********************************************************************/

    Parser::Parser()
    {

    }

    Parser::~Parser()
    {
        std::vector<SNode*>::iterator i = _streams.begin();
        for (; i != _streams.end(); ++i)
        {
            delete *i;
        }
        _streams.clear();
    }

    void Parser::parse(SNode* stream, CacheFile_Iterator &iter, int limit)
    {
        char check;
        std::cout << "   --> New stream with inner type " << std::hex << stream->type() << std::endl;
        while (!iter.atEnd() && limit != 0)
        {
            std::cout << "Stream len: " << stream->members().size() << " Limit " << limit
                      << " bytelimit " << iter.limit();
            std::cout << std::endl;
            check = iter.readChar() & 0x3f; // magic
            switch(check) {
            case ENone:
            {
                stream->addMember(new SNone());
            }
            break;
            case EInteger:
            {
                unsigned int val = iter.readInt();
                stream->addMember(new SInt(val));
            }
            break;
            case ELongLong:
            {
                long long val = iter.readLongLong();
                stream->addMember(new SLongLong(val));
            }
            break;
            case EShort:
            {
                int i = iter.readShort();
                stream->addMember(new SInt(i));
            }
            break;
            case EByte:
            {
                int i = iter.readChar();
                stream->addMember(new SInt(i));
            }
            break;
            case ESizedInt:
            {
                unsigned char len = iter.readChar();
                if (len == 8)
                    stream->addMember(new SLongLong(iter.readLongLong()));
                else if (len == 4)
                    stream->addMember(new SInt(iter.readInt())); // not observed
                else if (len == 2)
                    stream->addMember(new SInt(iter.readShort())); // not observed
            }
            break;
            case EIdent:
            {
                unsigned int len = iter.readChar();
                std::string data = iter.readString(len);
                stream->addMember(new SIdent(data));
            }
            break;
            case EString:
            {
                int len = iter.readChar();
                std::string data = iter.readString(len);
                stream->addMember(new SString(data));

                if (len == 0) {
                    // HACK HACK HACK - 0 length string is probably the end of this substream
                    // lets just give up now
                    std::cout << " from position " << std::hex << iter.position() << std::dec << std::endl;
                    while(!iter.atEnd())
                        iter.readChar();
                    return;
                }
            }
            break;
            case EDict:
            {
                unsigned int len = iter.readChar();
                SDict* dict = new SDict(len * 2); // key & val
                stream->addMember(dict);
                parse(dict, iter, len * 2);
            }
            break;
            case ETuple2:
            case ETuple:
            {
                unsigned int len = iter.readChar();
                STuple* tuple = new STuple(len);
                stream->addMember(tuple);
                parse(tuple, iter, len);

            }
            break;
            case E2Tuple:
            {
                STuple* tuple = new STuple(2);
                stream->addMember(tuple);
                parse(tuple, iter, 2);

            }
            break;
            case E1Tuple:
            {
                STuple* tuple = new STuple(1);
                stream->addMember(tuple);
                parse(tuple, iter, 1);

            }
            break;
            case E0Tuple:
            {
                STuple* tuple = new STuple(0);
                stream->addMember(tuple);
            }
            break;
            case EMarker:
            {
                char t = iter.readChar();
                stream->addMember(new SMarker(t));
            }
            break;
            case EObject:
            {
                SObject *obj = new SObject();
                stream->addMember(obj);
                parse(obj, iter, 2);
            }
            break;
            case EDBHeader:
            {
                SNode *obj = new SDBHeader();
                stream->addMember(obj);
                parse(obj, iter, 1);
    
            }
            break;
            case ESubstream:
            {
                unsigned int len = iter.readChar();
                if ((len & 0xff) == 0xFF)
                    len = iter.readInt();
                char sig = iter.readChar(); // 0x7e

                assert(sig == 0x7e);
                CacheFile_Iterator iter_sub(iter);
                iter_sub.setLimit(len);
                SSubstream *ss = new SSubstream(len);
                stream->addMember(ss);
                parse(ss, iter_sub, -1);
                iter.seek(iter_sub.position());
            }
            break;
            case ECompressedRow:
            {
                iter.readChar(); // Datatype 0x1b - special magic id?
                int magic = iter.readChar();  // Data for 0x1b
                int len = iter.readChar() & 0xFF;
                std::string compdata = iter.readString(len);

                const unsigned char* olddata = reinterpret_cast<const unsigned char*>
                    (compdata.c_str());

                std::vector<unsigned char> newdata;
                rle_unpack(olddata, len, newdata);

                stream->addMember(new SDBRow(magic, newdata));

            }
            break;
            case EDBRecords:
            {
                SDBRecords* rec = new SDBRecords();
                stream->addMember(rec);
                parse(rec, iter, -1);
            }
            break;
            case EStreamIdent:
            {
                int id = iter.readChar();
                stream->addMember(new SStreamIdent(id));
            }
            break;
            case 0x2d:
            {
                if(iter.readChar() != 0x2d) {
                    std::stringstream msg;
                    msg << "Didn't encounter a double 0x2d where I thought there should be one at " << iter.position();
                    throw ParseException(msg.str());
                }
                stream->addMember(new SString("2D2D MAGIC"));
                //return;
            }
            break;
            default:
            {
                if (iter.limit() == 0xa && check == 0x0)
                {
                    while(!iter.atEnd())
                        iter.readChar();
                    // HACK HACK - valid end of file, in bizarro CCP land?
                    return;
                }
                std::stringstream msg;
                msg << "Can't identify type 0x" << std::hex << static_cast<unsigned int>(check)
                    << " at position 0x" << iter.position() << " limit " << iter.limit();
                throw ParseException(msg.str());
            }
            }
            limit -= 1;
        }
        std::cout << " <-- parser exiting bytelimit : " <<
            iter.limit() << " inner type " << std::hex << stream->type() << std::endl;

    }

    void Parser::parse(CacheFile_Iterator& iter)
    {

        while(!iter.atEnd()) {
            char check = iter.readChar();
            SNode* stream = new SNode(EStreamStart);
            if (check != EStreamStart)
                throw ParseException("No stream start detected...");

            _streams.push_back(stream);
            parse(stream, iter, -1); // -1 = not sure how long this will be, just keep on going...


        }

    }

    std::vector<SNode*> Parser::streams() const
    {
        return _streams;
    }


};