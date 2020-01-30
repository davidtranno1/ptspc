#include "params.h"
#include <boost/filesystem.hpp>

namespace stok {
    Params::Params(const Instance& instance, po::parser& p)
        : instance{instance}
    {
        using namespace boost::filesystem;

        const auto solver_opt = p["solver"].get().string;

        if(solver_opt == "enum") {
            solver = SolverType::ENUM;
        } else if(solver_opt == "bck") {
            solver = SolverType::BCK;
        } else if(solver_opt == "fwd") {
            solver = SolverType::FWD;
        } else if(solver_opt == "bckfwd") {
            solver = SolverType::BCKFWD;
        } else if(solver_opt == "fwdbck") {
            solver = SolverType::FWDBCK;
        } else if(solver_opt == "tsp") {
            // Do nothing
        } else {
            throw std::logic_error("Invalid solver: " + solver_opt);
        }

        const auto exp_solver_opt = p["exp-cost-solver"].get().string;

        if(exp_solver_opt == "exact") {
            exp_cost_solver = ExpCostSolverType::EXACT;
        } else if(exp_solver_opt == "mc") {
            exp_cost_solver = ExpCostSolverType::MC;
        } else if(exp_solver_opt == "rtlr") {
            exp_cost_solver = ExpCostSolverType::RTLR;
        } else if(exp_solver_opt == "rt") {
            exp_cost_solver = ExpCostSolverType::RT;
        } else {
            throw std::logic_error(
                "Invalid exp cost solver: " + exp_solver_opt);
        }

        const auto tsp_solver_opt = p["tsp-solver"].get().string;

        if(tsp_solver_opt == "discorde") {
            tsp_solver = TspSolverEngine::DISCORDE;
        } else if(tsp_solver_opt == "cplexbc") {
            tsp_solver = TspSolverEngine::CPLEX_BC;
        }

        mc_iterations = p["monte-carlo-n"].get().u32;
        ml_training_time_s = p["ml-training-time"].get().f32;
        
        if(p["create-dataset"].available()) {
            const auto instance_name =
                path(p["instance"].get().string).stem().string();

            dataset_file = std::ofstream{
                "data-" +
                std::to_string(instance.number_of_vertices()) +
                "-" + solver_opt +
                "-" + exp_solver_opt +
                "-" + instance_name + ".csv"};
        }

        if(p["recompute-exact"].available()) {
            compute_exact_cost = true;
        }

        if(p["randomise-sets"].available()) {
            randomise_sets = true;
        }
    }

    Params::~Params() {
        if(dataset_file) {
            dataset_file->close();
        }
    }
}