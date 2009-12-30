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


#ifndef _EC_MARKET_H_
#define _EC_MARKET_H_

#include "evecache/config.hpp"

#include <vector>
#include <string>

namespace EveCache {


    class SNode;

    /**
     * Domain object for a market order
     */
    class EVECACHE_API MarketOrder {
    public:

        MarketOrder() {}

        /**
         * Utility
         */

        /**
         * Return this item as a standard CSV file export line
         */
        std::string toCsv() const;

        /**
         * Data setters
         */

        void setPrice(unsigned long long v) { _price = v; }
        void setVolRemaining(double v) { _volRemaining = v; }
        void setRange(unsigned int v) { _range = v; }
        void setOrderID(unsigned long long v) { _orderID = v; }
        void setVolEntered(unsigned int v) { _volEntered = v; }
        void setMinVolume(unsigned int v) { _minVolume = v; }
        void setBid(bool b) { _bid = b; }
        void setIssued(unsigned long long v) { _issued = v; }
        void setDuration(unsigned int v) { _duration = v; }
        void setStationID(unsigned int v) { _stationID = v; }
        void setRegionID(unsigned int v) { _regionID = v; }
        void setSolarSystemID(unsigned int v) { _solarSystemID = v; }
        void setJumps(unsigned int v) { _jumps = v; }
        void setType(unsigned int v) { _type = v; }

        /**
         * Data accessors
         */
        unsigned long long price() const { return _price; }
        double volRemaining() const { return _volRemaining; }
        unsigned int range() const { return _range; }
        unsigned long long orderID() const { return _orderID; }
        unsigned int volEntered() const { return _volEntered; }
        unsigned int minVolume() const { return _minVolume; }
        bool isBid() const  { return _bid; }
        unsigned long long issued() const  { return _issued; }
        unsigned int duration() const  { return _duration; }
        unsigned int stationID() const  { return _stationID; }
        unsigned int regionID() const { return _regionID; }
        unsigned int solarSystemID() const  { return _solarSystemID; }
        unsigned int jumps() const  { return _jumps; }
        unsigned int type() const { return _type; }

    private:
        unsigned long long _price;
        double _volRemaining;
        unsigned int _range;
        unsigned long long _orderID;
        unsigned int _volEntered;
        unsigned int _minVolume;
        bool _bid;
        unsigned long long _issued;
        unsigned int _duration;
        unsigned int _stationID;
        unsigned int _solarSystemID;
        unsigned int _regionID;
        unsigned int _jumps;
        unsigned int _type;
    };


    class EVECACHE_API MarketList {
    public:
        MarketList(int type, int region);
        MarketList();
        MarketList(const MarketList &rhs) {
            _type = rhs._type;
            _region = rhs._region;
            _sellOrders = rhs._sellOrders;
            _buyOrders = rhs._buyOrders;
            _ts = rhs._ts;
        }


        void setType(int v) { _type = v; }
        void setRegion(int r) { _region = r; }
        void setTimestamp(unsigned long t) { _ts = t; }

        int type() const { return _type; }
        int region() const { return _region; }
        unsigned long timestamp() const { return _ts; }

        std::vector<MarketOrder> getSellOrders() const { return _sellOrders; }
        std::vector<MarketOrder> getBuyOrders() const { return _buyOrders; }

        void addOrder(MarketOrder& order);

    private:
        int _type;
        int _region;
        unsigned long _ts;
        std::vector<MarketOrder> _sellOrders;
        std::vector<MarketOrder> _buyOrders;
    };


    class EVECACHE_API MarketParser {
    public:
        MarketParser(const SNode *stream);
        MarketParser(const char* fileName);
        MarketParser(const std::string fileName);
        ~MarketParser();
        MarketList getList() const;
        void parse();
        bool valid() const;
    private:
        const SNode *_stream;
        bool _valid;
        void initWithFile(const std::string& fileName);
        void parse(const SNode* nest);
        void parseDbRow(const SNode* nest);
        MarketList _list;

    };


};


#endif
