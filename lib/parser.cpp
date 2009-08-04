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
#include <sstream>

namespace EveCache {


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


/***********************************************************************/

    SDBHeader::SDBHeader() : SNode(EDBHeader)
    {
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

/***********************************************************************/


    SMarker::SMarker(char i) : SNode(EMarker), _id(i)
    {
    }

    char SMarker::id() const
    {
        return _id;
    }

/***********************************************************************/

    SIdent::SIdent(const std::string& n) : SNode(EIdent), _name(n)
    {
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


/***********************************************************************/

    SInt::SInt(int val) : SNode(EInteger), _value(val)
    {
    }

    int SInt::value() const
    {
        return _value;
    }

/***********************************************************************/

    SLongLong::SLongLong(long long val) : SNode(ELongLong), _value(val)
    {
    }

    long long SLongLong::value() const
    {
        return _value;
    }


/***********************************************************************/

    SObject::SObject() : SNode(EObject)
    {
    }


/***********************************************************************/

    SNone::SNone() : SNode(ENone)
    {
    }


/***********************************************************************/

    SSubstream::SSubstream(int len) : SNode(ENone), _len(len)
    {
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
                int val = iter.readInt();
                stream->addMember(new SInt(val));
            }
            break;
            case ELongLong:
            {
                long long val = iter.readLongLong();
                stream->addMember(new SLongLong(val));
            }
            break;
            case EIdent:
            {
                int len = iter.readChar();
                std::string data = iter.readString(len);
                stream->addMember(new SIdent(data));
            }
            break;
            case EString:
            {
                int len = iter.readChar();
                std::string data = iter.readString(len);
                stream->addMember(new SString(data));
            }
            break;
            case EDict:
            {
                int len = iter.readChar();
                SDict* dict = new SDict(len * 2); // key & val
                stream->addMember(dict);
                parse(dict, iter, len * 2);
            }
            break;
            case ETuple2:
            case ETuple:
            {
                int len = iter.readChar();
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
            case EMarker:
            {
                char t = iter.readChar();
                stream->addMember(new SMarker(t));
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
                char c = iter.readChar();
                if (c == 0x2d) {
                    c = iter.readChar();
                    if (c == 0x2d) {
                        std::cout << " Valid end of dbheader? " << std::endl;
                    }
                }
            }
            break;
            case ESubstream:
            {
                int len = iter.readChar();
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
            default:
                std::stringstream msg;
                msg << "Can't identify type 0x" << std::hex << static_cast<unsigned int>(check) 
                    << " at position 0x" << iter.position();
                throw ParseException(msg.str());
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
