%module libevecache

%{
#include <vector>
#include "evecache/reader.hpp"
#include "evecache/market.hpp"
    %}

%include "std_string.i"
%include "std_vector.i"


%include "evecache/reader.hpp"
%include "evecache/market.hpp"

namespace std {
    %template (MarketOrderVector) vector<EveCache::MarketOrder>;
     %template (UnsignedCharVector) vector<unsigned char>;
 }
