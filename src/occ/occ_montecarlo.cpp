#include "occ_montecarlo.h"

#include <cassert>
#include <cmath>
#include <random>

namespace stok {
    OfferedCost OCCMontecarlo::operator()(const CustomerSet& offered) const {
        assert(offered.size() == params.instance.number_of_deliveries());

        const auto offered_sz = std::count(
            offered.begin(), offered.end(), true);

        if(std::pow(2, offered_sz) <= params.mc_iterations) {
            return occ_exact(offered);
        }

        float total = 0.0f;
        float visited_prob = 0.0f;
        std::mt19937 mt;
        std::bernoulli_distribution bern;

        for(auto i = 0u; i < params.mc_iterations; ++i) {
            // Generate a random subset A of O.
            CustomerSet accepted(
                params.instance.number_of_deliveries(), false);
            
            for(auto i = 0u; i < offered.size(); ++i) {
                if(offered[i] && bern(mt)) {
                    accepted[i] = true;
                }
            }

            // Get C_r(O, A) for the random A generated.
            const auto [prob, val] = this->acc(offered, accepted);
            total += val;
            visited_prob += prob;
        }

        // Rescaling factor 1/visited_prob to take into account we
        // didn't visit all sets, but only some whose weights
        // (probabilities) sum up to visited_prob.
        const auto mc_estimate = 1.0f / visited_prob * total;

        if(dataset_for_csv) {
            dataset_for_csv->add_row_for(offered, mc_estimate);
        }

        return {mc_estimate, false};
    }
}