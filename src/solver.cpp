#include "solver.h"
#include "occ/occ_exact.h"
#include "occ/occ_montecarlo.h"
#include "occ/occ_ml_rt.h"
#include "occ/occ_ml_rtlr.h"
#include "solvers/solver_enum.h"
#include "solvers/solver_fwd.h"
#include "solvers/solver_bck.h"
#include "solvers/solver_fwdbck.h"
#include "solvers/solver_bckfwd.h"

#include <memory>

namespace stok {
    Solver::Solver(const Params& p) : params{p} {
        if(p.exp_cost_solver == ExpCostSolverType::EXACT) {
            occ = std::make_unique<const OCCExact>(p);
        } else if(p.exp_cost_solver == ExpCostSolverType::MC) {
            occ = std::make_unique<OCCMontecarlo>(p);
        } else if(p.exp_cost_solver == ExpCostSolverType::RT) {
            occ = std::make_unique<OCCMLRT>(p);
        } else if(p.exp_cost_solver == ExpCostSolverType::RTLR) {
            occ = std::make_unique<OCCMLRTLR>(p);
        } else {
            throw std::logic_error(
                "Expected cost solver currently not implemented!");
        }
    }

    std::unique_ptr<const Solver> get_solver(const Params& p) {
        if(p.solver == SolverType::ENUM) {
            return std::make_unique<const SolverEnum>(p);
        } else if(p.solver == SolverType::FWD) {
            return std::make_unique<const SolverFwd>(p);
        } else if(p.solver == SolverType::BCK) {
            return std::make_unique<const SolverBck>(p);
        } else if(p.solver == SolverType::FWDBCK) {
            return std::make_unique<const SolverFwdBck>(p);
        } else if(p.solver == SolverType::BCKFWD) {
            return std::make_unique<const SolverBckFwd>(p);
        } else {
            throw std::logic_error("Solver currently not implemented!");
        }
    }
}