#ifndef STOK_ACCEPTED_COST_CALCULATOR
#define STOK_ACCEPTED_COST_CALCULATOR

#include "params.h"
#include "tsp_solver.h"
#include "customer_set.h"

namespace stok {
    struct AcceptedCost {
        float probability;
        float value;
    };

    struct AcceptedCostCalculator {
        const Params& params;
        const TspSolver tsp_solver;

        explicit AcceptedCostCalculator(const Params& p) :
            params{p}, tsp_solver{p} {}

        AcceptedCost operator()(
            const CustomerSet& offered, const CustomerSet& accepted) const;
    };
}

#endif