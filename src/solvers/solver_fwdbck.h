#ifndef STOK_SOLVER_FWDBCK
#define STOK_SOLVER_FWDBCK

#include "../solver.h"
#include "../params.h"
#include "../solution.h"
#include "improving_customer.h"

namespace stok {
    struct SolverFwdBck : public Solver {
        explicit SolverFwdBck(const Params& p) : Solver{p} {}
        Solution operator()() const override;
        ~SolverFwdBck() override = default;
    };
}

#endif