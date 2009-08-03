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
        msg << "<SNode type " << type() << ">";
        return msg.str();
    }

    SNode::SNode(EStreamCode t) : _type(t)
    {
    }

    EStreamCode SNode::type() const
    {
        return _type;
    }

    void SNode::setType(EStreamCode t)
    {
        _type = t;
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
        _members = rhs._members;
    }

    void SStreamNode::addMember(SNode node)
    {
        _members.push_back(node);
    }

    std::vector<SNode> SStreamNode::members() const
    {
        return _members;
    }

/***********************************************************************/

    STuple::STuple(unsigned int len) : SStreamNode(ETuple), _givenLength(len)
    {
    }

    void STuple::addMember(SNode node)
    {
        assert(_members.size() < _givenLength);
        _members.push_back(node);
    }

/***********************************************************************/

    SDict::SDict(unsigned int len) : SStreamNode(EDict), _givenLength(len)
    {
    }

    void SDict::addMember(SNode node)
    {
        assert(_members.size() < _givenLength);
        _members.push_back(node);
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

    SInt::SInt(int val) : SNode(EInteger), _value(val)
    {
    }

    int SInt::value() const
    {
        return _value;
    }


/***********************************************************************/
    Parser::Parser()
    {

    }

    void Parser::parse(SStreamNode& stream, CacheFile_Iterator &iter, int limit)
    {
        char check;
        std::cout << "  New stream with inner type " << std::hex << stream.type() << std::endl;
        while (!iter.atEnd() || limit != 0)
        {
            std::cout << "Stream len: " << stream.members().size() << std::endl;
            check = iter.readChar();
            switch(check) {
            case EInteger: 
            {
                int val = iter.readInt();
                stream.addMember(static_cast<SNode>(SInt(val)));
            }
            break;
            case EIdent:
            {
                int len = iter.readChar();
                std::string data = iter.readString(len);
                stream.addMember(static_cast<SNode>(SIdent(data)));
            }
            break;
            case EDict:
            {
                int len = iter.readChar();
                SDict dict = SDict(len * 2); // key & val
                parse(dict, iter, len * 2);
                stream.addMember(static_cast<SNode>(dict));
            }
            break;
            case ETuple:
            {
                int len = iter.readChar();
                STuple tuple = STuple(len);
                parse(tuple, iter, len);
                stream.addMember(static_cast<SNode>(tuple));
            }
            break;
            case EMarker:
            {
                char t = iter.readChar();
                stream.addMember(static_cast<SNode>(SMarker(t)));
            }
            break;
            default:
                std::stringstream msg;
                msg << "Can't identify type " << std::hex << static_cast<int>(check) << std::dec
                    << " at position " << iter.position();
                throw ParseException(msg.str());
            }
            limit -= 1;
        }



    }

    void Parser::parse(CacheFile_Iterator& iter)
    {

        while(!iter.atEnd()) {
            char check = iter.readChar();
            SStreamNode stream;
            if (check != EStreamStart)
                throw ParseException("No stream start detected...");
            try {
                parse(stream, iter, -1); // -1 = not sure how long this will be, just keep on going...
            } catch (ParseException e) {
                // Trampoline the exception
                _streams.push_back(stream);
                throw e;
            }

            _streams.push_back(stream);
        }

    }

    std::vector<SStreamNode> Parser::streams() const
    {
        return _streams;
    }


};
