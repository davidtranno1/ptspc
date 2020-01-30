//
// Created by alberto on 17/05/19.
//

#ifndef STOCHASTIC_TSP_CACHE_H
#define STOCHASTIC_TSP_CACHE_H

#include "../customer_set.h"
#include <vector>
#include <cstdint>
#include <ext/pb_ds/assoc_container.hpp>

namespace stok {
    struct CustomerSetTraits {
        using size_type = CustomerSet::size_type;
        using key_type = CustomerSet;
        using key_const_reference = const CustomerSet&;
        using const_iterator = CustomerSet::const_iterator;
        using e_type = bool;
        enum { max_size = 99 };

        inline static const_iterator begin(key_const_reference k) { return k.begin(); }
        inline static const_iterator end(key_const_reference k) { return k.end(); }
        inline static std::size_t e_pos(e_type e) { return static_cast<std::size_t>(e); }
    };

    // Cache implementation using a Patricia Trie.
    using Cache = __gnu_pbds::trie<CustomerSet, float, CustomerSetTraits>;
}

#endif //STOCHASTIC_TSP_CACHE_H
