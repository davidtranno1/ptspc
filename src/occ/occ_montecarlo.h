#ifndef STOK_OCC_MONTECARLO
#define STOK_OCC_MONTECARLO

#include "../offered_cost_calculator.h"
#include "../customer_set.h"
#include "occ_exact.h"

namespace stok {
    struct OCCMontecarlo : public OfferedCostCalculator {
        OCCExact occ_exact; // Use it for small sets.

        explicit OCCMontecarlo(const Params& p) :
            OfferedCostCalculator{p}, occ_exact{p} {}

        OfferedCost operator()(const CustomerSet& offered) const override;
        ~OCCMontecarlo() override = default;
    };
}

#endif