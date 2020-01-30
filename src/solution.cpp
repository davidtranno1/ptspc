#include "solution.h"
#include "occ/occ_exact.h"
#include <chrono>
#include <iostream>

namespace stok {
    Solution create_sol_with_exact_cost(
        const Params& p, CustomerSet offered, float cost, float time)
    {
        Solution sol{p};

        sol.offered = offered;
        sol.cost = cost;
        sol.exact_cost = cost;
        sol.elapsed_time = time;
        sol.time_to_compute_exact = 0.0f;

        return sol;
    }

    Solution create_sol_with_approx_cost(
        const Params& p, CustomerSet offered, float cost, float time)
    {
        Solution sol{p};

        sol.offered = offered;
        sol.cost = cost;
        sol.elapsed_time = time;

        if(p.compute_exact_cost) {
            using namespace std::chrono;

            const auto start_time = high_resolution_clock::now();

            sol.exact_cost = OCCExact{p}(offered).cost;

            const auto end_time = high_resolution_clock::now();
            sol.time_to_compute_exact = 
                duration_cast<duration<double>>(end_time - start_time).count();
        }

        return sol;
    }

    std::ostream& operator<<(std::ostream& out, const Solution& sol) {
        const auto n_offered = std::count(
            sol.offered.begin(), sol.offered.end(), true);

        out <<
            n_offered << "," <<
            sol.cost << "," <<
            sol.elapsed_time << ",";
            
        if(sol.exact_cost) {
            assert(sol.time_to_compute_exact);
            out <<
                *(sol.exact_cost) << "," <<
                *(sol.time_to_compute_exact);
        } else {
            out << "-1,-1";
        }

        return out;
    }
}