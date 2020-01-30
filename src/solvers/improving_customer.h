#ifndef STOK_IMPROVING_CUSTOMER
#define STOK_IMPROVING_CUSTOMER

#include "../params.h"
#include "../customer_set.h"
#include "../offered_cost_calculator.h"

#include <optional>

namespace stok {
    struct ImprovingCustomer {
        std::uint32_t customer;
        OfferedCost cost;
    };

    std::optional<ImprovingCustomer> check_fwd_improving(
        const Params& p,
        const OfferedCostCalculator *const occ,
        const CustomerSet& best_set,
        const OfferedCost& best_cost,
        const std::optional<ImprovingCustomer> last_removed_customer =
            std::nullopt);
    
    std::optional<ImprovingCustomer> check_bck_improving(
        const Params& p,
        const OfferedCostCalculator *const occ,
        const CustomerSet& best_set,
        const OfferedCost& best_cost,
        const std::optional<ImprovingCustomer> last_added_customer =
            std::nullopt);
}

#endif