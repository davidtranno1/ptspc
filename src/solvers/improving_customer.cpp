#include "improving_customer.h"

#include <cassert>
#include <cstdint>
#include <as/numeric.h>

namespace stok {
    std::optional<ImprovingCustomer> check_fwd_improving(
        const Params& p,
        const OfferedCostCalculator *const occ,
        const CustomerSet& best_set,
        const OfferedCost& best_cost,
        const std::optional<ImprovingCustomer> last_removed_customer
        )
    {
        const auto n = p.instance.number_of_deliveries();
        const auto sz = std::count(best_set.begin(), best_set.end(), true);
        // Stupidly std::count returns a signed type!!

        assert(sz >= 0);
        assert(
            (as::numeric::can_type_fit_value<std::uint32_t, decltype(sz)>(sz)));

        if(static_cast<std::uint32_t>(sz) == n) {
            return std::nullopt;
        }

        std::optional<std::uint32_t> improving_customer = std::nullopt;
        std::optional<OfferedCost> improved_cost = std::nullopt;

        for(auto i = 0u; i < n; ++i) {
            if(last_removed_customer && last_removed_customer->customer == i) {
                continue;
            }

            if(best_set[i]) {
                continue;
            }

            CustomerSet new_set = best_set;
            new_set[i] = true;

            const auto new_cost = (*occ)(new_set);

            if(new_cost < best_cost) {
                if(!improved_cost || new_cost < *improved_cost) {
                    improving_customer = i;
                    improved_cost = new_cost;
                }
            }
        }

        if(improving_customer) {
            assert(*improving_customer < n);
            assert(improved_cost);

            return ImprovingCustomer{*improving_customer, *improved_cost};
        }

        return std::nullopt;
    }

    std::optional<ImprovingCustomer> check_bck_improving(
        const Params& p,
        const OfferedCostCalculator *const occ,
        const CustomerSet& best_set,
        const OfferedCost& best_cost,
        const std::optional<ImprovingCustomer> last_added_customer
        )
    {
        const auto n = p.instance.number_of_deliveries();
        const auto sz = std::count(best_set.begin(), best_set.end(), true);

        if(sz == 0) {
            return std::nullopt;
        }

        std::optional<std::uint32_t> improving_customer = std::nullopt;
        std::optional<OfferedCost> improved_cost = std::nullopt;

        for(auto i = 0u; i < n; ++i) {
            if(last_added_customer && last_added_customer->customer == i) {
                continue;
            }

            if(!best_set[i]) {
                continue;
            }

            CustomerSet new_set = best_set;
            new_set[i] = false;

            const auto new_cost = (*occ)(new_set);

            if(new_cost < best_cost) {
                if(!improved_cost || new_cost < *improved_cost) {
                    improving_customer = i;
                    improved_cost = new_cost;
                }
            }
        }

        if(improving_customer) {
            assert(*improving_customer < n);
            assert(improved_cost);

            return ImprovingCustomer{*improving_customer, *improved_cost};
        }

        return std::nullopt;
    }
}