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
    private:
        std::vector<SStreamNode> _streams;
    };




};

#endif
