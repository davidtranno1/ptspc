#include "tsp_solver.h"
#include "utils/disable_stream.h"
#include "utils/tsp_bc.h"
#include <as/tsp.h>
#include <cassert>
#include <stdexcept>

namespace stok {
    float TspSolver::operator()(const CustomerSet& accepted) const {
        const auto& instance = params.instance;

        assert(accepted.size() == instance.number_of_deliveries());

        const auto it = cache.find(accepted);

        // If the cost was in the cache, simply return it.
        if(it != cache.end()) { return it->second; }

        // Otherwise, compute it and store it in the cache.

        float cost = 0.0f;

        for(auto i = 0u; i < accepted.size(); ++i) {
            if(accepted[i]) {
                cost += instance.outsource_cost[i + 1u];
            }
        }

        auto tsp_customers = accepted;
        tsp_customers.flip();

        DISABLE_STREAM(stdout, 1)
        DISABLE_STREAM(stderr, 2)

        if(params.tsp_solver == TspSolverEngine::DISCORDE) {
            cost += as::tsp::tour_cost(instance, as::tsp::solve_with_depot(instance, tsp_customers));
        } else if(params.tsp_solver == TspSolverEngine::CPLEX_BC) {
            cost += bc_tsp_solve(instance, tsp_customers);
        } else {
            throw std::runtime_error("Cannot solve TSP with requested method.");
        }

        REENABLE_STREAM(stdout, 1)
        REENABLE_STREAM(stderr, 2)

        cache[accepted] = cost;
        return cost;
    }
}