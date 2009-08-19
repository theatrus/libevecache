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


#ifndef _EC_MARKET_H_
#define _EC_MARKET_H_

#include <vector>

namespace EveCache {

    /**
     * Domain object for a market order
     */
    class MarketOrder {
    public:

        /**
         * Yes this is a constructor of doom
         */
        MarketOrder(unsigned long long price, unsigned int volr, unsigned int range,
                    unsigned long long orderid, unsigned int minv, bool ibid,
                    unsigned long long issued, unsigned int dur, unsigned int station,
                    unsigned int solarsys, unsigned int jumps) : _price(price), _volRemaining(volr),
                                                                 _range(range), _orderID(orderid),
                                                                 _minVolume(minv), _bid(ibid),
                                                                 _issued(issued), _duration(dur),
                                                                 _stationID(station),
                                                                 _solarSystemID(solarsys),
                                                                 _jumps(jumps)
            {}

        /**
         * Data accessors
         */
        unsigned long long price() const { return _price; }
        unsigned int volRemaining() const { return _volRemaining; }
        unsigned int range() const { return _range; }
        unsigned long long orderID() const { return _orderID; }
        unsigned int minVolume() const { return _minVolume; }
        bool isBid() const  { return _bid; }
        unsigned long long issued() const  { return _issued; }
        unsigned int duration() const  { return _duration; }
        unsigned int stationID() const  { return _stationID; }
        unsigned int solarSystemID() const  { return _solarSystemID; }
        unsigned int jumps() const  { return _jumps; }

    private:
        unsigned long long _price;
        unsigned int _volRemaining;
        unsigned int _range;
        unsigned long long _orderID;
        unsigned int _minVolume;
        bool _bid;
        unsigned long long _issued;
        unsigned int _duration;
        unsigned int _stationID;
        unsigned int _solarSystemID;
        unsigned int _jumps;
    };


    class MarketList {
    public:
        MarketList(int type, int region);
    private:
        std::vector<MarketOrder> _sellOrders;
        std::vector<MarketOrder> _buyOrders;
    };



};


#endif
