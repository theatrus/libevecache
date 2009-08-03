

#include "evecache/reader.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Cache File Dumper " << std::endl;
    if (argc < 2) {
        std::cerr << "Error: Syntax: " << argv[0] << " [filename]" << std::endl;
        return -1;
    }

    {
        using namespace EveCache;
        std::string fileName(argv[1]);
        CacheFile cF(fileName);
        cF.readFile();
        std::cout << "File length is " << cF.getLength() << " bytes " << std::endl;
        CacheFile_Iterator i = cF.begin();
        for (; i != cF.end(); i+= 1)
        {
            std::cout << i.peekInt();
        }
    }
    
}
