#ifndef STOK_TSP_SOLVER
#define STOK_TSP_SOLVER

#include "utils/cache.h"
#include "params.h"

namespace stok {
    struct TspSolver {
        const Params& params;
        mutable Cache cache;

        explicit TspSolver(const Params& p) : params{p} {}

        float operator()(const CustomerSet& accepted) const;
    };
}

#endif