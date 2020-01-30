#ifndef STOK_SOLVER_BCK
#define STOK_SOLVER_BCK

#include "../solver.h"
#include "../params.h"
#include "../solution.h"
#include "improving_customer.h"

namespace stok {
    struct SolverBck : public Solver {
        explicit SolverBck(const Params& p) : Solver{p} {}
        Solution operator()() const override;
        ~SolverBck() override = default;
    };
}

#endif