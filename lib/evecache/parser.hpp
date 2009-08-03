#ifndef _EC_PARSER_H_
#define _EC_PARSER_H_

#include <vector>

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

    class SNode {
    public:
        virtual ~SNode() = 0;
    protected:
        EStreamCode type;
    };

    class SStreamNode : public SNode {
    public:
        SStreamNode();
    protected:
        std::vector<SNode> members;
    };


};

#endif
