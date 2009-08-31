// libevecache - EVE Cache File Reader Library
// Copyright (C) 2009  StackFoundry LLC and Yann Ramin
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
// http://www.stackfoundry.com/libevecache
// http://gitorious.org/libevecache

#include "evecache/reader.hpp"
#include "evecache/parser.hpp"
#include "evecache/market.hpp"
#include "evecache/exceptions.hpp"

#include <iostream>
#include <cstring>

// Yes I'm lazy
using namespace EveCache;

void nspaces(int n)
{
    n *= 2;
    for (int i = 0; i < n; i++) {
        std::cout << " "; // horrendously inefficient 4tw
    }
}

void dump(const std::vector<SNode*>& stream, int level)
{
    std::vector<SNode*>::const_iterator vi = stream.begin();

    //std::cout << " {Length: " << stream.size() << "} ";

    for (; vi != stream.end(); ++vi)
    {
        nspaces(level);
        std::cout << "" << ((*vi)->repl()) << " " << std::endl;
        if ((*vi)->members().size() > 0) { // generic catch all members with nested members
            const SNode* sn = *vi;
            const std::vector<SNode*>& ste = sn->members();
            nspaces(level);
            std::cout << " (" << std::endl;

            dump(ste, level + 1);

            nspaces(level);
            std::cout << " )" << std::endl;
        }
    }

}


void market(const SNode* node)
{
    MarketParser mp(node);
    mp.parse();
}

int main(int argc, char** argv)
{
    std::cout << "Cache File Dumper " << std::endl;
    if (argc < 2) {
        std::cerr << "Error: Syntax: " << argv[0] << " [options] [filename]" << std::endl;
        return -1;
    }
    bool dumpStructure = false;
    bool dumpMarket = false;
    // Parse options in simple mode
    if (argc > 2) {
        for (int i = 1; i < argc - 2; i++)
        {
            if (strcmp(argv[i], "--market") == 0) {
                dumpMarket = true;
            }
            if (strcmp(argv[i], "--structure") == 0) {
                dumpStructure = true;
            }
        }
    } else {
        dumpStructure = true; // default
    }


    std::cout << "File: " << argv[argc-1] << std::endl;
    {
        std::string fileName(argv[argc-1]);
        CacheFile cF(fileName);
        cF.readFile();
        std::cout << "File length is " << cF.getLength() << " bytes " << std::endl;
        CacheFile_Iterator i = cF.begin();
        Parser *parser = new Parser(&i);
        try {
            parser->parse();
        } catch (ParseException e) {
            std::cout << "Parse exception " << static_cast<std::string>(e) << std::endl;
        }

        if (dumpStructure) {
            // TODO; more than one stream
            for (int i = 0; i < parser->streams().size(); i++) {
                const std::vector<SNode*>& streams = parser->streams()[i]->members();
                std::cout << "Beginning dump..." << std::endl;
                dump(streams, 0);
            }
        }
        if (dumpMarket) {
            std::cout << "MARKET INFORMATION" << std::endl;
            for (int i = 0; i < parser->streams().size(); i++) {
                const SNode* snode = parser->streams()[i];
                std::cout << "Beginning dump..." << std::endl;
                market(snode);
            }

        }
    }
    std::cout << std::endl;
}
