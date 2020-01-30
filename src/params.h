#ifndef STOK_PARAMS
#define STOK_PARAMS

#include <ProgramOptions.hxx>
#include "instance.h"

#include <optional>
#include <fstream>
#include <cstdint>

namespace stok {
    enum class SolverType {
        ENUM, BCK, FWD, FWDBCK, BCKFWD
    };

    enum class ExpCostSolverType {
        EXACT, MC, RTLR, RT
    };

    enum class TspSolverEngine {
        DISCORDE,
        CPLEX_BC
    };

    struct Params {
        const Instance& instance;

        mutable std::optional<std::ofstream> dataset_file;

        SolverType solver = SolverType::ENUM;
        ExpCostSolverType exp_cost_solver = ExpCostSolverType::EXACT;
        TspSolverEngine tsp_solver = TspSolverEngine::DISCORDE;

        std::uint32_t mc_iterations = 20u;
        float ml_training_time_s = 60.0;
        bool compute_exact_cost = false;
        bool randomise_sets = false;

        Params(const Instance& instance, po::parser& p);
        ~Params();
    };
}

#endif