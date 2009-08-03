#include "evecache/parser.hpp"
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

    SIdent::SIdent(const std::string& n) : name(n)
    {
        type = SIdent;
    }

    std::string SIdent::getName() const
    {
        return name;
    }




};
