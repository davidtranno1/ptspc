#ifndef STOK_SOLUTION
#define STOK_SOLUTION

#include "customer_set.h"
#include "params.h"
#include <optional>
#include <iostream>

namespace stok {
struct Solution {
        const Params& params;

        // Set O represented by the solution.
        CustomerSet offered;

        // Exact or heuristic cost.
        float cost;

        // Exact cost, if available.
        std::optional<float> exact_cost = std::nullopt;

        // Time to compute the cost.
        float elapsed_time;

        // Given the set O, additional time needed to compute
        // the exact cost when this was not available in advance.
        std::optional<float> time_to_compute_exact = std::nullopt;

        Solution(const Params& p) : params{p} {}
    };

    Solution create_sol_with_exact_cost(
        const Params& p, CustomerSet offered, float cost, float time);

    Solution create_sol_with_approx_cost(
        const Params& p, CustomerSet offered, float cost, float time);

    std::ostream& operator<<(std::ostream& out, const Solution& sol);
}

#endif