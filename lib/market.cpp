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

    void MarketParser::parse()
    {
        /* Step 1: Determine if this is a market order file */
        SNode *base = _stream->members()[0];
        SIdent *id = dynamic_cast<SIdent*>(base->members()[0]->members()[1]);
    }

    MarketList MarketParser::getList() const
    {
        return _list;
    }
};
