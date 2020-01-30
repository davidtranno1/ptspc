#ifndef STOK_OFFERED_COST_CALCULATOR
#define STOK_OFFERED_COST_CALCULATOR

#include "params.h"
#include "dataset.h"
#include "accepted_cost_calculator.h"

#include <optional>
#include <vector>

namespace stok {
    struct OfferedCost {
        float cost;
        bool is_exact;

        bool operator<(const OfferedCost& other) const {
            // Compare costs regardless of whether or not they are exact.
            return cost < other.cost;
        }
    };

    struct OfferedCostCalculator {
        const Params& params;
        const AcceptedCostCalculator acc;

        mutable std::optional<CsvDataset> dataset_for_csv = std::nullopt;
        mutable std::optional<PyDataset> dataset_for_py = std::nullopt;

        explicit OfferedCostCalculator(const Params& p);

        virtual OfferedCost operator()(const CustomerSet& offered) const = 0;

        virtual ~OfferedCostCalculator();
    };
}

#endif