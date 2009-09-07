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
    try {
        mp.parse();
    } catch (ParseException &e) {
        std::cerr << "Not a valid orders file" << std::endl;
        return;
    }
    MarketList list = mp.getList();
    std::cerr << "MarketList for region " << list.region() << " and type " << list.type() << std::endl;
    std::cout << "price,volRemaining,typeID,range,orderID,volEntered,minVolume,bid,issued,duration,stationID,regionID,solarSystemID,jumps," << std::endl;
    std::vector<MarketOrder> buy = list.getBuyOrders();
    std::vector<MarketOrder> sell = list.getSellOrders();
    std::vector<MarketOrder>::const_iterator i = sell.begin();
    for (; i != sell.end(); ++i)
    {
        std::cout << (*i).toCsv() << std::endl;
    }
    i = buy.begin();
    for (; i != buy.end(); ++i)
    {
        std::cout << (*i).toCsv() << std::endl;
    }

}

int main(int argc, char** argv)
{
    std::cerr << "Cache File Dumper " << std::endl;
    if (argc < 2) {
        std::cerr << "Error: Syntax: " << argv[0] << " [options] [filename]" << std::endl;
        return -1;
    }
    bool dumpStructure = false;
    bool dumpMarket = false;
    int argsconsumed = 1;

    // Parse options in simple mode
    if (argc > 2) {
        for (int i = 1; i < argc - 1; i++)
        {
            if (strcmp(argv[i], "--market") == 0) {
                dumpMarket = true;
                argsconsumed++;
            }
            if (strcmp(argv[i], "--structure") == 0) {
                dumpStructure = true;
                argsconsumed++;
            }
        }
    } else {
        dumpStructure = true; // default
    }


    for (int filen = argsconsumed; filen < argc - 1; filen++)
    {
        std::cerr << "File: " << argv[filen] << std::endl;
        {
            std::string fileName(argv[filen]);
            CacheFile cF(fileName);
            cF.readFile();
            std::cerr << "File length is " << cF.getLength() << " bytes " << std::endl;
            CacheFile_Iterator i = cF.begin();
            Parser *parser = new Parser(&i);

            try {
                parser->parse();
            } catch (ParseException e) {
                std::cerr << "Parse exception " << static_cast<std::string>(e) << std::endl;
            }

            if (dumpStructure) {
                // TODO; more than one stream
                for (int i = 0; i < parser->streams().size(); i++) {
                    const std::vector<SNode*>& streams = parser->streams()[i]->members();
                    dump(streams, 0);
                }
            }
            if (dumpMarket) {
                for (int i = 0; i < parser->streams().size(); i++) {
                    const SNode* snode = parser->streams()[i];
                    market(snode);
                }

            }
            delete parser;
        }
        std::cout << std::endl;
    }
}
