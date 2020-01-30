#ifndef STOK_SOLVER_FWD
#define STOK_SOLVER_FWD

#include "../solver.h"
#include "../params.h"
#include "../solution.h"
#include "improving_customer.h"

namespace stok {
    struct SolverFwd : public Solver {
        explicit SolverFwd(const Params& p) : Solver{p} {}
        Solution operator()() const override;
        ~SolverFwd() override = default;
    };
}

#endif