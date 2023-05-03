#include <ext/pb_ds/assoc_container.hpp>

template <typename K, typename V>
using FastHashMap = __gnu_pbds::gp_hash_table<K, V>;

template <typename K, typename V>
using FastLookupHashMap = __gnu_pbds::cc_hash_table<K, V>;
