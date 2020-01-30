#ifndef STOK_SOLVER
#define STOK_SOLVER

#include "params.h"
#include "solution.h"
#include "offered_cost_calculator.h"

#include <memory>

namespace stok {
    struct Solver {
        const Params& params;
        std::unique_ptr<const OfferedCostCalculator> occ;

        explicit Solver(const Params& p);
        virtual Solution operator()() const = 0;
        virtual ~Solver() = default;
    };

    std::unique_ptr<const Solver> get_solver(const Params& p);
}

#endif