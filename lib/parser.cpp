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
        msg << "<SNode type " << type << ">";
        return msg.str();
    }

/***********************************************************************/



    SStreamNode::SStreamNode() : SNode()
    {
        type = EStreamStart;
    }

    SStreamNode::SStreamNode(const SStreamNode& rhs)
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

    STuple::STuple(unsigned int len) : SStreamNode(), _givenLength(len)
    {
        type = ETuple;
    }

    void STuple::addMember(SNode node)
    {
        assert(_members.size() < _givenLength);
        _members.push_back(node);
    }

/***********************************************************************/


    SMarker::SMarker(char i) : _id(i)
    {
        type = EMarker;
    }

    char SMarker::id() const
    {
        return _id;
    }

/***********************************************************************/

    SIdent::SIdent(const std::string& n) : SNode(), _name(n)
    {
        type = EIdent;
    }

    std::string SIdent::name() const
    {
        return _name;
    }

/***********************************************************************/

    SInt::SInt(int val) : _value(val)
    {
        type = EInteger;
    }

    int SInt::value() const
    {
        return _value;
    }


/***********************************************************************/
    Parser::Parser()
    {

    }

    void Parser::parse(CacheFile_Iterator& iter)
    {
        // Find one stream
        // TODO: Nested streams...
        char check = iter.readChar();
        SStreamNode stream;
        if (check != EStreamStart)
            throw ParseException("No stream start detected...");

        while (!iter.atEnd())
        {
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

            default:
                std::stringstream msg;
                msg << "Can't identify type " << static_cast<int>(check)
                    << " at position " << iter.position();
                _streams.push_back(stream);
                throw ParseException(msg.str());
            }
        }

        _streams.push_back(stream);

    }

    std::vector<SStreamNode> Parser::streams() const
    {
        return _streams;
    }


};
