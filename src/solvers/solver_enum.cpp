#include "solver_enum.h"
#include "../solution.h"
#include "../offered_cost_calculator.h"

#include <chrono>
#include <stdexcept>
#include <as/combinatorial.h>

namespace stok {
    Solution SolverEnum::operator()() const {
        using namespace std::chrono;

        CustomerSet best_set = {};
        OfferedCost best_cost = {std::numeric_limits<float>::max(), true};

        const auto update_best = [&] (const CustomerSet &offered) -> void {
            const auto exp_cost = (*occ)(offered);

            if(exp_cost < best_cost) {
                best_cost = exp_cost;
                best_set = offered;
            }
        };

        const auto start_time = high_resolution_clock::now();

        as::combi::visit_subsets(
            params.instance.number_of_deliveries(), &update_best);

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