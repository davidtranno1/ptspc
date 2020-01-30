#ifndef STOK_SOLVER_BCKFWD
#define STOK_SOLVER_BCKFWD

#include "../solver.h"
#include "../params.h"
#include "../solution.h"
#include "improving_customer.h"

namespace stok {
    struct SolverBckFwd : public Solver {
        explicit SolverBckFwd(const Params& p) : Solver{p} {}
        Solution operator()() const override;
        ~SolverBckFwd() override = default;
    };
}

#endif