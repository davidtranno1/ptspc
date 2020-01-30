//
// Created by alberto on 17/05/19.
//

#ifndef STOCHASTIC_TSP_INSTANCE_H
#define STOCHASTIC_TSP_INSTANCE_H

#include <vector>
#include <string>
#include <as/tsplib.h>

namespace stok {
    struct Instance : as::tsplib::TSPInstance {
        // Note: acceptance_prob and outsource_cost contain n+1 entries,
        // where n is the number of customers. In other words, at index 0
        // they contain dummy values for the depot.

        std::string filename;
        std::vector<float> acceptance_prob;
        std::vector<float> outsource_cost;

        explicit Instance(const std::string& filename) :
            TSPInstance{filename},
            filename{filename}
        {
            acceptance_prob = tsp.get_data("ACCEPTED_PROBABILITIES");
            outsource_cost = tsp.get_data("OUTSOURCING_COSTS");
        }

        std::size_t number_of_deliveries() const {
            return number_of_vertices() - 1u;
        }
    };
}

#endif //STOCHASTIC_TSP_INSTANCE_H
