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

    void SStreamNode::addMember(SNode &node)
    {
        members.push_back(node);
    }

/***********************************************************************/

    STupleNode::STupleNode(unsigned int len) : SNode(), givenLength(len)
    {
        type = ETuple;
    }

    void STupleNode::addMember(SNode& node)
    {
        assert(members.size() < givenLength);
        members.push_back(node);
    }

    unsigned int STupleNode::getGivenLength()
    {
        return givenLength;
    }

/***********************************************************************/


    SMarker::SMarker(char i) : id(i)
    {
        type = EMarker;
    }

    char SMarker::getId() const
    {
        return id;
    }

/***********************************************************************/

    SIdent::SIdent(const std::string& n) : SNode(), name(n)
    {
        type = EIdent;
    }

    std::string SIdent::getName() const
    {
        return name;
    }


/***********************************************************************/
    Parser::Parser()
    {

    }

    void Parser::parse(CacheFile_Iterator& iter)
    {
        // Find one stream
        char check = iter.readChar();
        SStreamNode stream();
        if (check != EStreamStart)
            throw ParseException("No stream start detected...");

        while (!iter.atEnd())
        {

        }

    }

    std::vector<SStreamNode> Parser::getStreams() const
    {
        return streams;
    }


};
