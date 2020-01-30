#include "solver_fwdbck.h"
#include "randomise_set.h"
#include "../solution.h"
#include "../offered_cost_calculator.h"

#include <chrono>
#include <stdexcept>
#include <as/combinatorial.h>

namespace stok {
    Solution SolverFwdBck::operator()() const {
        using namespace std::chrono;

        const auto n = params.instance.number_of_deliveries();
        const auto start_time = high_resolution_clock::now();

        auto best_set = CustomerSet(n, false);

        if(params.randomise_sets) {
            randomise(best_set);
        }

        auto best_cost = (*occ)(best_set);

        std::optional<ImprovingCustomer> improve_fwd = std::nullopt;
        std::optional<ImprovingCustomer> improve_bck = std::nullopt;

        while(true) {
            improve_fwd = check_fwd_improving(
                params, occ.get(), best_set, best_cost, improve_bck);

            if(improve_fwd) {
                best_set[improve_fwd->customer] = true;
                best_cost = improve_fwd->cost;
            }

            if(std::count(best_set.begin(), best_set.end(), true) == 1) {
                if(improve_fwd) {
                    continue;
                } else {
                    break;
                }
            }

            improve_bck = check_bck_improving(
                params, occ.get(), best_set, best_cost, improve_fwd);

            if(improve_bck) {
                best_set[improve_bck->customer] = false;
                best_cost = improve_bck->cost;
            }

            if(!improve_fwd && !improve_bck) {
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