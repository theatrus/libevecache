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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://dev.eve-central.com/libevecache/
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
//std::cerr << "RLE: t zero " << count << std::endl;
                for (; count > 0; count--)
                {
                    buffer.push_back(0);
                }
            } else {
                unsigned int count = 8 - opcap.tlen;
//std::cerr << "RLE: t copy " << count << std::endl;
                for (; count > 0 && in_buf < end ; count--) {
                    buffer.push_back(*in_buf++);
                }
            }
            if (opcap.bzero) {
                unsigned char count = opcap.blen + 1;
//std::cerr << "RLE: b zero " << count << std::endl;
                for (; count > 0; count--)
                {
                    buffer.push_back(0);
                }
            } else {
                unsigned int count = 8 - opcap.blen;
//std::cerr << "RLE: b copy " << count << std::endl;
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
        std::vector<SNode*>::const_iterator i = rhs._members.begin();
        for ( ; i != rhs._members.end(); ++i)
        {
            _members.push_back((*i)->clone());
        }
        _type = rhs._type;
    }

    SNode* SNode::clone() const
    {
        return new SNode(*this);
    }

    SNode::~SNode()
    {
        std::vector<SNode*>::iterator i = _members.begin();
        for (; i != _members.end(); ++i)
        {
            delete *i;
        }
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

    SStreamNode* SStreamNode::clone() const
    {
        return new SStreamNode(*this);
    }

/***********************************************************************/

    SDBHeader::SDBHeader() : SNode(ECompressedRow)
    {
    }

    std::string SDBHeader::repl() const
    {
        return std::string("<SDBHeader>");
    }


    SDBHeader* SDBHeader::clone() const
    {
        return new SDBHeader(*this);
    }

/***********************************************************************/


    STuple::STuple(unsigned int len) : SNode(ETuple), _givenLength(len)
    {
    }

    STuple::STuple(const STuple &rhs) : SNode(rhs)
    {
        _givenLength = rhs._givenLength;
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

    STuple* STuple::clone() const
    {
        return new STuple(*this);
    }


/***********************************************************************/

    SDict::SDict(unsigned int len) : SNode(EDict), _givenLength(len)
    {
    }

    SDict::SDict(const SDict &rhs) : SNode(rhs)
    {
        _givenLength = rhs._givenLength;
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

    SDict* SDict::clone() const
    {
        return new SDict(*this);
    }

    SNode* SDict::getByName(const std::string &target) const
    {
        if (_members.size() < 2 || _members.size() & 1)
            return NULL;

        /* Non idiomatic C++ coming, you've been warned */
        /* Optimization improvement: replace this thing with a real map */

        for (unsigned int i = 1; i < _members.size(); i += 2) {
            SIdent *name = dynamic_cast<SIdent*>(_members[i]);
            if (name != NULL) {
                if (name->name() == target)
                    return _members[i - 1];
            }
        }
        return NULL;
    }


/***********************************************************************/


    SMarker::SMarker(unsigned char i) : SNode(EMarker), _id(i)
    {
    }

    std::string SMarker::repl() const
    {
        std::stringstream ss;
        ss << " <SMarker ID: " << static_cast<unsigned int>(id()) << " '" << string() << "' > ";
        return ss.str();
    }

    unsigned char SMarker::id() const
    {
        return _id;
    }

    std::string SMarker::string() const
    {
        std::string name = ColumnLookup::lookupName(id());
        if (name.empty())
        {
            std::stringstream ss;
            ss << "UNKNOWN:" << static_cast<unsigned int>(id());
            return ss.str();
        } else
            return name;
    }

    SMarker* SMarker::clone() const
    {
        return new SMarker(*this);
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


    SIdent* SIdent::clone() const
    {
        return new SIdent(*this);
    }

/***********************************************************************/

    SString::SString(const std::string& n) : SNode(EString), _name(n)
    {
    }

    std::string SString::string() const
    {
        return _name;
    }

    std::string SString::repl() const
    {
        std::stringstream ss;
        ss << " <SString '" << string() << "'> ";
        return ss.str();
    }

    SString* SString::clone() const
    {
        return new SString(*this);
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

    SInt* SInt::clone() const
    {
        return new SInt(*this);
    }


/***********************************************************************/

    SReal::SReal(double val) : SNode(EReal), _value(val)
    {
    }

    double SReal::value() const
    {
        return _value;
    }

    std::string SReal::repl() const
    {
        std::stringstream ss;
        ss << " <SReal '" << value() << "'> ";
        return ss.str();
    }

    SReal* SReal::clone() const
    {
        return new SReal(*this);
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

    SLongLong* SLongLong::clone() const
    {
        return new SLongLong(*this);
    }


/***********************************************************************/

    SObject::SObject() : SNode(EObject)
    {
    }

    std::string SObject::name() const
    {
        const SNode *cur = this;
        while (cur->members().size())
            cur = static_cast<SNode*>(cur->members()[0]);

        const SString *str = dynamic_cast<const SString*>(cur);

        if (str != NULL)
            return str->string();
        return std::string("");
    }

    std::string SObject::repl() const
    {
        std::stringstream ss;
        ss << " <SObject '" << name() << "' " << this << "> ";
        return ss.str();
    }


    SObject* SObject::clone() const
    {
        return new SObject(*this);
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

    SNone* SNone::clone() const
    {
        return new SNone(*this);
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

    SSubstream* SSubstream::clone() const
    {
        return new SSubstream(*this);
    }

/***********************************************************************/

    SDBRow::SDBRow(int magic, const std::vector<unsigned char>& data)
        : SNode(ECompressedRow), _id(magic), _last(false), _data(data)
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

        if (isLast())
            ss << " LAST ";

        ss << ">";
        return ss.str();
    }

    bool SDBRow::isLast() const
    {
        return _last;
    }

    void SDBRow::setLast(bool last)
    {
        _last = last;
    }

    SDBRow* SDBRow::clone() const
    {
        return new SDBRow(*this);
    }



/***********************************************************************/

    SDBRecords::SDBRecords() : SNode(ECompressedRow)
    {
    }

    std::string SDBRecords::repl() const
    {
        return std::string("");
    }

    SDBRecords* SDBRecords::clone() const
    {
        return new SDBRecords(*this);
    }

/***********************************************************************/

    Parser::Parser(CacheFile_Iterator *iter)
        : _iter(iter), _sharecount(0), _sharecursor(0), _shareobj(NULL), _sharemap(NULL)
    {
    }

    Parser::~Parser()
    {

        std::vector<SNode*>::iterator i = _streams.begin();
        for (; i != _streams.end(); ++i)
        {
            delete *i;
        }

        if (_shareobj != NULL)
            for (unsigned int j = 0; j <= _sharecount; j++) {
                if (_shareobj[j] != NULL)
                    delete _shareobj[j];
            }


        if (_shareobj != NULL)
            delete [] _shareobj;
        if (_sharemap != NULL)
            delete [] _sharemap;
    }

    SNode* Parser::parseone()
    {
        char check;
        char isshared = 0;
        SNode *thisobj = NULL;
        SDBRow *lastDbRow = NULL;

        try {
            char type = _iter->readChar();
            check = type & 0x3f;
            isshared = type & 0x40;
        } catch (EndOfFileException &e) {
            return NULL;
        }

//std::cerr << "DEB: parseone, pos " << _iter->position() << ", type " << std::hex << static_cast<unsigned int>(check) << ", shared " << static_cast<unsigned int>(isshared) << std::endl;

        switch(check) {
        case ENone:
        {
            thisobj = new SNone();
        }
        break;
        case EReal:
        {
            double val = _iter->readDouble();
            thisobj = new SReal(val);
        }
        break;
        case E0Real:
        {
            thisobj = new SReal(0);
        }
        break;
        case EInteger:
        {
            unsigned int val = _iter->readInt();
            thisobj = new SInt(val);
        }
        break;
        case EBoolFalse:
        case E0Integer:
        {
            thisobj = new SInt(0);
        }
        break;
        case EBoolTrue: /* Replace with a real Bool node one day */
        case E1Integer:
        {
            thisobj = new SInt(1);
        }
        break;
        case ENeg1Integer:
        {
            thisobj = new SInt(-1);
        };
        break;
        case ELongLong:
        {
            long long val = _iter->readLongLong();
            thisobj = new SLongLong(val);
        }
        break;
        case EShort:
        {
            int i = _iter->readShort();
            thisobj = new SInt(i);
        }
        break;
        case EByte:
        {
            int i = _iter->readChar();
            thisobj = new SInt(i);
        }
        break;
        case ESizedInt:
        {
            unsigned char len = _iter->readChar();
            if (len == 8)
                thisobj = new SLongLong(_iter->readLongLong());
            else if (len == 4)
                thisobj = new SInt(_iter->readInt()); // not observed
            else if (len == 2)
                thisobj = new SInt(_iter->readShort()); // not observed
            else if (len == 3)
                thisobj = new SInt((_iter->readChar()) + (_iter->readChar() << 16) + (_iter->readChar() << 24));
        }
        break;
        case EIdent:
        {
            unsigned int len = getLen();
            std::string data = _iter->readString(len);
            thisobj = new SIdent(data);
        }
        break;
        case EEmptyString:
        {
            thisobj = new SString("");
        }
        break;
        case EUnicodeString2:
        {
            /* Single unicode character */
            std::string data = _iter->readString(2);
            thisobj = new SString(data);
        }
        break;
        case EString3:
        {
            std::string data = _iter->readString(1);
            thisobj = new SString(data);
        }
        break;
        case E0String:
        {
            std::string data;
            thisobj = new SString(data);
        }
        break;
        case EUnicodeString:
        case EString4:
        case EString2:
        case EString:
        {
            int len = _iter->readChar();
            std::string data = _iter->readString(len);
            thisobj = new SString(data);

//            if (len == 0 && (_iter->limit() - _iter->position()) <= 0xf) {
                // HACK HACK HACK - 0 length string is probably the end of this substream
                // lets just give up now
//                while(!_iter->atEnd())
//                   _iter->readChar();
//                return;
//            }
        }
        break;
        case EDict:
        {
            unsigned int len = getLen();
            SDict* dict = new SDict(len * 2); // key & val
            thisobj = dict;
            try {
                parse(dict, len * 2);
            } catch (ParseException &e) {
                delete dict;
                throw e;
            } catch (EndOfFileException &e) {
                delete dict;
                throw e;
            }
        }
        break;
        case ETuple2:
        case ETuple:
        {
            unsigned int len = getLen();
            thisobj = new STuple(len);
            try {
                parse(thisobj, len);
            } catch (ParseException &e) {
                delete thisobj;
                throw e;
            } catch (EndOfFileException &e) {
                delete thisobj;
                throw e;
            }

        }
        break;
        case E2Tuple:
        {
            thisobj = new STuple(2);
            try {
                parse(thisobj, 2);
            } catch (ParseException &e) {
                delete thisobj;
                throw e;
            } catch (EndOfFileException &e) {
                delete thisobj;
                throw e;
            }

        }
        break;
        case E1Tuple2:
        case E1Tuple:
        {
            thisobj = new STuple(1);
            try {
                parse(thisobj, 1);
            } catch (ParseException &e) {
                delete thisobj;
                throw e;
            } catch (EndOfFileException &e) {
                delete thisobj;
                throw e;
            }

        }
        break;
        case E0Tuple2:
        case E0Tuple:
        {
            thisobj = new STuple(0);
        }
        break;
        case EMarker:
        {
            unsigned int t = getLen();
            thisobj = new SMarker(t);
        }
        break;
        case EObject:
        {
            SObject *obj = new SObject();
            thisobj = obj;
            try {
                parse(obj, 2);
            } catch (ParseException &e) {
                delete obj;
                throw e;
            } catch (EndOfFileException &e) {
                delete obj;
                throw e;
            }
        }
        break;
        case EObject22:
        case EObject23:
        {
            SObject *obj = new SObject();
            thisobj = obj;
            try {
                parse(obj, 1);
            } catch (ParseException &e) {
                delete obj;
                throw e;
            } catch (EndOfFileException &e) {
                delete obj;
                throw e;
            }

            std::string oclass(obj->name());
//std::cerr << "Obj: " << obj << " == " << obj->repl() << ", class " << oclass << std::endl;

// HALP
// out of ideas at this point
// some kind of hook for class-specific object processing
// needs to be attached here, that handles the objects depeding
// on the classname which should be in cur at this point.
//
// f.ex. dbutil.RowList -> keep reading rows until you hit the 0x2d marker
//
            if (! oclass.compare("dbutil.RowList")) {
                try {
                    SNode *row;
                    while ( (row = parseone()) ) {
                        obj->addMember(row);
                    }
                } catch (ParseException &e) {
                    delete obj;
                    throw e;
                } catch (EndOfFileException &e) {
                    delete obj;
                    throw e;
                }
            }

        }
        break;
        case ESubstream:
        {
            unsigned int len = getLen();
            //char sig = _iter->readChar(); // 0x7e
            //assert(sig == 0x7e);
            CacheFile_Iterator iter_sub(*_iter);
            iter_sub.setLimit(len);
            SSubstream *ss = new SSubstream(len);
            thisobj = ss;
            try {
                Parser sp(&iter_sub);
                sp.parse();
                for (unsigned int i = 0; i < sp.streams().size(); i++) {
                    ss->addMember(sp.streams()[i]->clone());
                }
            } catch (ParseException &e) {
                delete ss;
                throw e;
            } catch (EndOfFileException &e) {
                delete ss;
                throw e;
            }

            _iter->seek(iter_sub.position());
        }
        break;
        case ECompressedRow:
        {
            try {
                thisobj = getDBRow();
            } catch (ParseException &e) {
                delete thisobj;
                throw e;
            } catch (EndOfFileException &e) {
                delete thisobj;
                throw e;
            }
        }
        break;
        case ESharedObj:
        {
            unsigned int id = getLen();
            thisobj = shareGet(id);
        }
        break;
        case EChecksum:
        {

            thisobj = new SString("checksum");
            _iter->readInt();
        }
        break;
        case 0x2d:
        {
            if(_iter->readChar() != 0x2d) {
                std::stringstream msg;
                msg << "Didn't encounter a double 0x2d where I thought there should be one at " << _iter->position();
                throw ParseException(msg.str());
            }
            if (lastDbRow)
                lastDbRow->setLast(true);
            return NULL;
        }
        break;
        case 0:
            break;
        default:
        {
//            if (_iter->limit() == 0xa && check == 0x0)
//            {
//                while(!_iter->atEnd())
//                    _iter->readChar();
                // HACK HACK - valid end of file, in bizarro CCP land?
//                return;
//            }
            std::stringstream msg;
            msg << "Can't identify type 0x" << std::hex << static_cast<unsigned int>(check)
                << " at position 0x" << _iter->position() << " limit " << _iter->limit();
            throw ParseException(msg.str());
        }
        }

        if (!thisobj) {
            throw ParseException("no thisobj in parseone");
        }

        if (isshared) {
            if (!thisobj) {
                throw ParseException("shared flag but no obj");
            }
            try {
                shareAdd(thisobj);
            } catch (ParseException &e) {
                delete thisobj;
                throw e;
            }

        }

        return thisobj;
    }

    void Parser::parse(SNode* stream, int limit)
    {
        while (!_iter->atEnd() && limit != 0)
        {
            SNode *thisobj = parseone();
            if (thisobj) {
                stream->addMember(thisobj);
            }
            limit -= 1;
        }

    }

    void Parser::parse()
    {

        try {
            while(!_iter->atEnd()) {
                char check = _iter->readChar();
                SNode* stream = new SNode(EStreamStart);

                if (check != EStreamStart) {
                    delete stream;
                    //throw ParseException("No stream start detected...");
                    continue;
                }

                _streams.push_back(stream);
                shareInit();
                parse(stream, 1); // -1 = not sure how long this will be

                shareSkip();
            }
        } catch (EndOfFileException &e) {
            // Ignore the exception, parser has run amok!
        }
    }

    SNode* Parser::getDBRow()
    {
        SNode *nhead = parseone();
        // get header
        SObject* head = dynamic_cast<SObject*>(nhead);
        if (head == NULL) {
            delete nhead;
            throw ParseException("The DBRow header isn't present...");
        }

        if (head->name().compare("blue.DBRowDescriptor")) {
            delete head;
            throw ParseException("bad descriptor name");
        }

        STuple* fields = dynamic_cast<STuple*>(head->members()[0]->members()[1]->members()[0]);
//std::cerr << "DBRow: fields: " << fields << " == " << fields->repl() << ", size " << static_cast<unsigned int>(fields->members().size()) << std::endl;

        unsigned int len = getLen();
        std::string compdata = _iter->readString(len);
        const unsigned char* olddata = reinterpret_cast<const unsigned char*>
            (compdata.c_str());

        std::vector<unsigned char> newdata;
        rle_unpack(olddata, len, newdata);
        SNode* body = new SDBRow(17, newdata);

        CacheFile cF(newdata);
        CacheFile_Iterator blob = cF.begin();

        SDict *dict = new SDict(999999); // TODO: need dynamic sized dict
        int step = 1;
        while (step < 6)
        {
            std::vector<SNode*>::const_iterator vi = fields->members().begin();

            for (; vi != fields->members().end(); ++vi)
            {
                SNode *fn = (*vi)->members()[0]->clone();
                SInt *ft = (SInt*)(*vi)->members()[1];
                int fti = ft->value();

//std::cerr << "DBRow: step " << step << ", type " << fti << ", name " << fn->repl() << std::endl;

                unsigned char boolcount=0;
                unsigned char boolbuf=0;
                SNode *obj=NULL;
                switch(fti) {
                case 21:
                case 20: if (step == 1) // 64bit int
                    {
                        long long val = blob.readLongLong();
                        obj = new SLongLong(val);
                    }
                    break;
                case 6: if (step == 1) // currency
                    {
                        long long val = blob.readLongLong();
                        obj = new SLongLong(val);
                    }
                    break;
                case 64: if (step == 1) // timestamp
                    {
                        long long val = blob.readLongLong();
                        obj = new SLongLong(val);
                    }
                    break;
                case 5: if (step == 1) // double
                    {
                        double val = blob.readDouble();
                        obj = new SReal(val);
                    }
                    break;
                case 4:
                {
                    double val = blob.readFloat();
                    obj = new SReal(val);
                }
                break;
                case 19:
                case 3: if (step == 2) // 32bit int
                    {
                        int val = blob.readInt();
                        obj = new SInt(val);
                    }
                    break;
                case 18:
                case 2: if (step == 3) // 16bit int
                    {
                        int val = blob.readShort();
                        obj = new SInt(val);
                    }
                    break;
                case 17:
                case 16: 
                {
                    int val = blob.readChar();
                    obj = new SInt(val);
                }
                break;
                case 11: if (step == 5) // boolean
                    {
                        if (!boolcount) {
                            boolbuf = blob.readChar();
                            //std::cerr << "Boolbuf" << (int)boolbuf << std::endl;
                            boolcount = 0x1;
                        }
                        if (boolbuf & boolcount) {
                            obj = new SInt(1);
                        } else {
                            obj = new SInt(0);
                        }
                        boolcount <<= 1;
                    }
                    break;
                case 129: // String types
                case 128:
                case 130:
                {
                    obj = new SString("I can't parse strings yet - be patient");
                }
                break;
                default:
                {
                    if (obj != NULL)
                        delete obj;
                    delete fn;
                    delete body;
                    delete head;
                    delete dict;
                    std::stringstream ss;
                    ss << "Unhandled ADO type " << fti;
                    throw ParseException(ss.str());
                }
                }
                if (obj) {
                    dict->addMember(obj);
                    dict->addMember(fn);
                } else {
                    delete fn;
                }
            }

            step++;

        }


        SNode* fakerow = new STuple(3);
        fakerow->addMember(head);
        fakerow->addMember(body);
        fakerow->addMember(dict);
        return fakerow;
    }

    unsigned int Parser::shareInit()
    {
        unsigned int shares = _iter->readInt();
        if (shares >= 16384) // Some large number
            return 0;

        unsigned int shareskip = 0;
        if (shares) {
            _sharemap = new unsigned int[shares+1];
            _shareobj = new SNode*[shares+1];

            shareskip = 4 * shares;
            unsigned int opos = _iter->position();
            unsigned int olim = _iter->limit();
            _iter->seek(opos + olim - shareskip);
            unsigned int i;
            for (i=0; i < shares; i++) {
                _sharemap[i] = _iter->readInt();
                _shareobj[i] = NULL;
            }
            _shareobj[shares] = NULL;
            _sharemap[shares] = 0;

            _iter->seek(opos);
            _iter->setLimit(olim - shareskip);
        }
        _sharecount = shares;
        return shares;
    }

    void Parser::shareAdd(SNode *obj)
    {
        if (_sharemap == NULL || _shareobj == NULL)
            throw ParseException("Uninitialized share");
        if (_sharecursor >= _sharecount)
            throw ParseException("cursor out of range");
        unsigned int shareid = _sharemap[_sharecursor];
        if (shareid > _sharecount)
            throw ParseException("shareid out of range");

        /** This is bad, but a memory leak fix until the sharetab support is better */
        if (_shareobj[shareid] != NULL) {
            //throw ParseException("already have obj");
            delete _shareobj[shareid];
        }
        _shareobj[shareid] = obj->clone();

        _sharecursor++;
    }

    SNode* Parser::shareGet(unsigned int id)
    {
        if (id > _sharecount) {
            std::stringstream ss;
            ss << "ShareID out of range " << id << " > " << _sharecount;
            throw ParseException(ss.str());
        }

        if (_shareobj[id] == NULL) {
            std::stringstream ss;
            ss << "ShareTab: No entry at position " << id;
            throw ParseException(ss.str());
        }

        return _shareobj[id]->clone();
    }

    void Parser::shareSkip()
    {
        _iter->advance(_sharecount*4);
    }

    int Parser::getLen()
    {
        unsigned int len = _iter->readChar();
        if ((len & 0xff) == 0xFF)
            len = _iter->readInt();
        return len;
    }

    std::vector<SNode*> Parser::streams() const
    {
        return _streams;
    }


};
