#include "randomise_set.h"

#include <random>

namespace stok {
    void randomise(CustomerSet& set) {
        std::mt19937 mt;
        std::bernoulli_distribution bern;

        for(auto i = 0u; i < set.size(); ++i) {
            set[i] = bern(mt);
        }
    }
}