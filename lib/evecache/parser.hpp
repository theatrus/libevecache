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

#include "evecache/config.hpp"

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

    protected:

    };

/***********************************************************************/

    class EVECACHE_API STuple : public SNode {
    public:
        STuple(unsigned int len);
        virtual ~STuple();
        virtual unsigned int givenLength() const;
        virtual void addMember(SNode* node);
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
    protected:
        unsigned int _givenLength;
    };


/***********************************************************************/

    class EVECACHE_API SNone : public SNode {
    public:
        SNone();
    };


/***********************************************************************/

    class EVECACHE_API SMarker : public SNode {
    public:
        SMarker(char id);
        virtual char id() const;
    protected:
        char _id;
    };

/***********************************************************************/

    class EVECACHE_API SIdent : public SNode {
    public:
        SIdent(const std::string& m);
        virtual std::string name() const;
    protected:
        std::string _name;
    };

/***********************************************************************/

    class EVECACHE_API SInt : public SNode {
    public:
        SInt(int val);
        virtual int value() const;
    private:
        int _value;
    };

/***********************************************************************/

    class EVECACHE_API SObject : public SNode {
    public:
        SObject();
    private:
    };




/***********************************************************************/
    class EVECACHE_API Parser {
    public:
        Parser();
        ~Parser();
        void parse(CacheFile_Iterator& iter);
        std::vector<SNode*> streams() const;
    protected:
        void parse(SNode* node, CacheFile_Iterator& iter, int limit);
    private:
        std::vector<SNode*> _streams;
    };




};

#endif
