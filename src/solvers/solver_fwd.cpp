#include "solver_fwd.h"
#include "randomise_set.h"
#include "../solution.h"
#include "../offered_cost_calculator.h"

#include <chrono>
#include <stdexcept>
#include <as/combinatorial.h>

namespace stok {
    Solution SolverFwd::operator()() const {
        using namespace std::chrono;

        const auto n = params.instance.number_of_deliveries();
        const auto start_time = high_resolution_clock::now();

        auto best_set = CustomerSet(n, false);

        if(params.randomise_sets) {
            randomise(best_set);
        }

        auto best_cost = (*occ)(best_set);

        while(true) {
            const auto improve = check_fwd_improving(
                params, occ.get(), best_set, best_cost);

            if(improve) {
                best_set[improve->customer] = true;
                best_cost = improve->cost;
            } else {
                break;
            }
        }

        const auto end_time = high_resolution_clock::now();
        const auto elapsed = duration_cast<duration<double>>(
            end_time - start_time).count();

        if(best_cost.is_exact) {
            return create_sol_with_exact_cost(
                params, best_set, best_cost.cost, elapsed);
        }

        return create_sol_with_approx_cost(
            params, best_set, best_cost.cost, elapsed);
    }
}