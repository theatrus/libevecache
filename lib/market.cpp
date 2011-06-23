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



#include "evecache/market.hpp"
#include "evecache/dbtypes.hpp"
#include "evecache/reader.hpp"
#include "evecache/parser.hpp"
#include "evecache/exceptions.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <time.h>

namespace EveCache {


    std::string MarketOrder::toCsv() const
    {
        std::stringstream ss;
        ss << std::fixed << std::showpoint << std::setprecision(2);
        ss << price() / 10000.0;

        ss << std::setprecision(1);
        ss << "," << volRemaining();
        ss << "," << type();
        ss << "," << range();
        ss << "," << orderID();
        ss << "," << volEntered();
        ss << "," << minVolume();

        if (isBid())
            ss << "," << "True";
        else
            ss << "," << "False";


        time_t t = windows_to_unix_time(issued());
        struct tm *tmp;

        tmp = gmtime(&t);

        char times[200];
        strftime(times, 200, "%Y-%m-%d %H:%M:%S", tmp);


        ss << "," << times << ".000";
        ss << "," << duration();
        ss << "," << stationID();
        ss << "," << regionID();
        ss << "," << solarSystemID();
        ss << "," << jumps();
        return ss.str();
    }

    MarketList::MarketList(int type, int region) : _type(type), _region(region)
    {
    }

    MarketList::MarketList() : _type(0), _region(0), _ts(0)
    {
    }

    void MarketList::addOrder(MarketOrder& order)
    {
        if (order.isBid())
            _buyOrders.push_back(order);
        else
            _sellOrders.push_back(order);
    }

    MarketList MarketParser::getList() const
    {
        return _list;
    }



    MarketParser::MarketParser(const SNode* stream) : _stream(stream), _valid(false)
    {

    }

    MarketParser::MarketParser(const char* fileName) : _valid(false)
    {
        try {
            initWithFile(std::string(fileName));
        } catch (ParseException &e) {
            return;
        }
    }

    MarketParser::MarketParser(const std::string fileName) : _valid(false)
    {
        try{
            initWithFile(fileName);
        } catch (ParseException &e) {
            return;
        }
    }

    MarketParser::~MarketParser()
    {
    }

    void MarketParser::initWithFile(const std::string& fileName)
    {
        CacheFile cF(fileName);
        if (cF.readFile() == false) {
            throw new ParseException("Can't open file" + fileName);
        }
        CacheFile_Iterator i = cF.begin();
        Parser *parser = new Parser(&i);
        parser->parse();
        const SNode* snode = parser->streams()[0];
        _stream = snode;
        parse();
        delete parser;
        _stream = NULL;
        _valid = true;
    }

    bool MarketParser::valid() const
    {
        return _valid;
    }


    void MarketParser::parseDbRow(const SNode* node)
    {
        MarketOrder order;

        std::vector<SNode*>::const_iterator i = node->members().begin();

        for (; i != node->members().end(); ++i)
        {


            SNode* value = *i;
            ++i;
            SMarker* key = dynamic_cast<SMarker*>(*i);
			SIdent* ident = dynamic_cast<SIdent*>(*i);

			int typeKey = -1;

			if (key != 0) {
				typeKey = key->id();
			} else {
				if (ident->name() == "issueDate") {
					typeKey = 131;
				} else {
					std::cerr << "Can't parse - giving up" << std::endl;
					break;
				}
			}



            SInt* intV = dynamic_cast<SInt*>(value);
            SLongLong* longV = dynamic_cast<SLongLong*>(value);
            SReal* realV = dynamic_cast<SReal*>(value);

            int sintV = 0;
            long long slongV = 0;
            double srealV = 0.0;

            if (longV != NULL)
                slongV = longV->value();

            if (intV != NULL)
                sintV = intV->value();

            if (realV != NULL)
                srealV = realV->value();


            switch(typeKey) {
            case 139:
                order.setPrice(slongV);
                break;
            case 161:
                order.setVolRemaining(srealV);
                break;
            case 131:
                order.setIssued(slongV);
                break;
            case 138:
                order.setOrderID(slongV);
                break;
            case 160:
                order.setVolEntered(sintV);
                break;
            case 137:
                order.setMinVolume(sintV);
                break;
            case 155:
                order.setStationID(sintV);
                break;
            case 141:
                order.setRegionID(sintV);
                _list.setRegion(sintV);
                break;
            case 150:
                order.setSolarSystemID(sintV);
                break;
            case 41:
                order.setJumps(sintV);
                break;
            case 74:
                order.setType(sintV);
                _list.setType(sintV);
                break;
            case 140:
                order.setRange(sintV);
                break;
            case 126:
                order.setDuration(sintV);
                break;
            case 116:
                if (sintV)
                    order.setBid(true);
                else
                    order.setBid(false);
                break;
			default:
				std::cout << "Unknown key ID:" << (int)key->id() <<  " r: " << srealV << " l: " << slongV << " i: " << sintV << std::endl;
				break;

            }
        }
        _list.addOrder(order);


    }

    void MarketParser::parse(const SNode* node)
    {
        if (node->members().size() > 0) {
            std::vector<SNode*>::const_iterator i = node->members().begin();
            for (; i!= node->members().end(); ++i) {
                SDBRow *dbrow = dynamic_cast<SDBRow*>(*i);
                if (dbrow != NULL) {
                    ++i;
                    parseDbRow(*i);
                } else {
                    parse(*i);
                }
            }
        }
    }

    void MarketParser::parse()
    {
        if (_stream == NULL)
            return;

        /* Todo: fixed offsets = bad :) */
        /* Step 1: Determine if this is a market order file */
        if (_stream->members().size() < 1)
            throw ParseException("Not a valid file");

        SNode *base = _stream->members()[0];

        if (base->members().size() < 1)
            throw ParseException("Not a valid orders file");
        if (base->members()[0]->members().size() < 2)
            throw ParseException("Not a valid orders file");

        SIdent *id = dynamic_cast<SIdent*>(base->members()[0]->members()[1]);
        if (id == NULL)
            throw ParseException("Can't determine method name");
        if (id->name() != "GetOrders")
            throw ParseException("Not a valid orders file");

        /* Retrieve the region and type */

        SInt *region = dynamic_cast<SInt*>(base->members()[0]->members()[2]);
        SInt *type = dynamic_cast<SInt*>(base->members()[0]->members()[3]);

        _list.setRegion(region->value());
        _list.setType(type->value());

        /* Try to extract the in-file timestamp */

        SDict *dict = dynamic_cast<SDict*>(base->members()[1]);
        if (dict == NULL)
            throw ParseException("Can't read file timestamp");

        // Grab the version entry of the version tuple
        SLongLong *time = dynamic_cast<SLongLong*>(dict->getByName("version")->members()[0]);
        if (time == NULL)
            throw ParseException("Can't read file timestamp");

        std::cout << "TS: " << time->value() << std::endl;
        _list.setTimestamp(windows_to_unix_time(time->value()));

        SNode *obj = dynamic_cast<SObject*>(base->members()[1]->members()[0]);
        if (obj == NULL)
            return;
        parse(obj);
        _valid = true;
    }

};
