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


#include "evecache/reader.hpp"
#include "evecache/parser.hpp"
#include "evecache/exceptions.hpp"

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
        Parser parser;
        try {
            parser.parse(i);
        } catch (ParseException e) {
            std::cout << "Parse exception " << static_cast<std::string>(e) << std::endl;
        }
        std::cout << "Beginning dump..." << std::endl;

        // TODO; more than one stream
        const std::vector<SNode> streams = parser.streams()[0].members();
        std::vector<SNode>::const_iterator vi = streams.begin();
        for (; vi != streams.end(); ++vi)
        {
            std::cout << (vi->repl()) << std::endl;
        }
    }
}
