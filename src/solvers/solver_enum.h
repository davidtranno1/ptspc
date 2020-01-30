#ifndef STOK_SOLVER_ENUM
#define STOK_SOLVER_ENUM

#include "../solver.h"
#include "../params.h"
#include "../solution.h"

namespace stok {
    struct SolverEnum : public Solver {
        explicit SolverEnum(const Params& p) : Solver{p} {}
        Solution operator()() const override;
        ~SolverEnum() override = default;
    };
}

#endif