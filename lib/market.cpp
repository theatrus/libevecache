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



#include "evecache/market.hpp"
#include "evecache/parser.hpp"
#include "evecache/exceptions.hpp"

#include <iostream>

namespace EveCache {
    MarketList::MarketList(int type, int region) : _type(type), _region(region)
    {
    }

    MarketList::MarketList() : _type(0), _region(0)
    {
    }

    void MarketList::addOrder(MarketOrder& order)
    {
        if (order.isBid())
            _buyOrders.push_back(order);
        else
            _sellOrders.push_back(order);
    }


    MarketParser::MarketParser(const SNode* stream) : _stream(stream)
    {

    }

    MarketParser::~MarketParser()
    {
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

            SInt* intV = dynamic_cast<SInt*>(value);
            SLongLong* longV = dynamic_cast<SLongLong*>(value);

            int sintV = 0;
            long long slongV = 0;
            if (longV != NULL)
                slongV = longV->value();

            if (intV != NULL)
                sintV = intV->value();

            switch(key->id()) {
            case 139:
                order.setPrice(slongV);
                break;
            case 161:
                order.setVolRemaining(sintV);
                break;
            case 131:
                order.setIssued(sintV);
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
                order.setBid(sintV);
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
        /* Step 1: Determine if this is a market order file */
        SNode *base = _stream->members()[0];
        SIdent *id = dynamic_cast<SIdent*>(base->members()[0]->members()[1]);
        if (id->name() != "GetOrders")
            throw ParseException("Not a valid orders file");

        /* Todo: fixed offsets = bad :) */

        SNode *obj = dynamic_cast<SObject*>(base->members()[1]->members()[0]); // Should be an SObject
        if (obj == NULL)
            throw ParseException("Can't find the SObject");
        parse(obj);
    }

    MarketList MarketParser::getList() const
    {
        return _list;
    }
};
