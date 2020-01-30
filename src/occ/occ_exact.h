#ifndef STOK_OCC_EXACT
#define STOK_OCC_EXACT

#include "../offered_cost_calculator.h"
#include "../customer_set.h"

namespace stok {
    struct OCCExact : public OfferedCostCalculator {
        explicit OCCExact(const Params& p) :
            OfferedCostCalculator{p} {}

        OfferedCost operator()(const CustomerSet& offered) const override;
        ~OCCExact() override = default;
    };
}

#endif