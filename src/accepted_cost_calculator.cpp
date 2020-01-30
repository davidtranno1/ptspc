#include "accepted_cost_calculator.h"

#include <cassert>

namespace stok {
    AcceptedCost AcceptedCostCalculator::operator()(
        const CustomerSet& offered, const CustomerSet& accepted) const
    {
        const auto& instance = params.instance;

        assert(offered.size() == instance.number_of_deliveries());
        assert(accepted.size() == instance.number_of_deliveries());

        float probability = 1.0f;

        for(auto i = 0u; i < accepted.size(); ++i) {
            if(offered[i]) {
                probability *= (accepted[i]) ?
                               instance.acceptance_prob[i + 1u] :
                               1.0f - instance.acceptance_prob[i + 1u];
            } else {
                // Could not be accepted, if it was not offered!
                assert(!accepted[i]);
            }
        }

        return {probability, probability * tsp_solver(accepted)};
    }
}