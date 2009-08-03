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
        virtual ~SStreamNode() { }
        void addMember(SNode &node);

    protected:
        std::vector<SNode> members;
    };

/***********************************************************************/

    class STupleNode : public SNode {
    public:
        STupleNode(unsigned int len);
        void addMember(SNode& node);
        unsigned int getGivenLength();
    protected:
        std::vector<SNode> members;
        unsigned int givenLength;
    };

/***********************************************************************/

    class SMarker : public SNode {
    public:
        SMarker(char id);
        char getId() const;
    protected:
        char id;
    };
    
/***********************************************************************/
    
    class SIdent : public SNode {
    public:
        SIdent(const std::string& m);
        std::string getName() const;
    protected:
        std::string name;
    };




/***********************************************************************/
    class Parser {
    public:
        Parser();
        void parse(CacheFile_Iterator& iter);
        std::vector<SStreamNode> getStreams() const;
    private:
        std::vector<SStreamNode> streams;
    };




};

#endif
