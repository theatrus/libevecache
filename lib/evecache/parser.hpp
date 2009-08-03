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

#ifndef _EC_PARSER_H_
#define _EC_PARSER_H_

#include <vector>
#include <string>



namespace EveCache {

    typedef enum {
        EStreamStart = 0x7e,
        ETuple = 0x14, // a tuple of N objects
        EMarker = 0x11, // A one byte identifier code
        EIdent = 0x13, // an identifier string
        EInteger = 0x04, // 4 byte, little endian?
        EShort = 0x05, // 2 byte
        EDict = 0x16, // N objects, consisting of key object and value object
        EObject = 0x17, // Object type ?
        ENone = 0x01, // Python None type
    } EStreamCode;


    class CacheFile_Iterator;

/***********************************************************************/

    class SNode {
    public:
        virtual ~SNode() {};
        virtual std::string repl() const;
    protected:
        EStreamCode type;
    };

/***********************************************************************/

    class SStreamNode : public SNode {
    public:
        SStreamNode();
        SStreamNode(const SStreamNode &rhs);
        virtual ~SStreamNode() { }
        virtual void addMember(SNode node);
        virtual std::vector<SNode> members() const;
    protected:
        std::vector<SNode> _members;
    };

/***********************************************************************/

    class STuple : public SStreamNode {
    public:
        STuple(unsigned int len);
        unsigned int givenLength();
        virtual void addMember(SNode node);
    protected:
        unsigned int _givenLength;
    };

/***********************************************************************/

    class SMarker : public SNode {
    public:
        SMarker(char id);
        char id() const;
    protected:
        char _id;
    };
    
/***********************************************************************/
    
    class SIdent : public SNode {
    public:
        SIdent(const std::string& m);
        std::string name() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class SInt : public SNode {
    public:
        SInt(int val);
        int value() const;
    private:
        int _value;
    };


/***********************************************************************/
    class Parser {
    public:
        Parser();
        void parse(CacheFile_Iterator& iter);
        std::vector<SStreamNode> streams() const;
    protected:
        void parse(SStreamNode& node, CacheFile_Iterator& iter);
    private:
        std::vector<SStreamNode> _streams;
    };




};

#endif
