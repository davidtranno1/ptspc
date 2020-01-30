#include "occ_exact.h"
#include <cassert>
#include <as/combinatorial.h>

namespace stok {
    OfferedCost OCCExact::operator()(const CustomerSet& offered) const {
        const auto& instance = params.instance;

        assert(offered.size() == instance.number_of_deliveries());

        float total = 0.0f;

        // Function that gets the expected value for a given a set of
        // accepted customers A.
        // Notice that we have to make a transformation, because of the
        // following. If O = { 2, 4, 7 }, accepted_small will be a vector
        // of three bools that tell us whether 2, 4, or 7 (repsectively)
        // were accepted. AcceptedCostCalculator::operator(), however,
        // expects A to be a full-size vector with as many entries as
        // customers. If there are 7 customers in the instance, then,
        // a vector accepted_small = { true, false, true } needs to become
        // { false, true, false, false, false, false, true }.
        //          ^            ^                    ^
        // where the marked elements correspond to customers 2, 4, 7.
        const auto exp_getter = [&](const CustomerSet &accepted_small) -> void {
            CustomerSet accepted(instance.number_of_deliveries(), false);

            std::size_t offered_num = 0u;
            for(auto i = 0u; i < accepted.size(); ++i) {
                if(offered[i]) {
                    if(accepted_small[offered_num]) {
                        accepted[i] = true;
                    }
                    ++offered_num;
                }
            }

            assert(offered_num == accepted_small.size());

            total += this->acc(offered, accepted).value;
        };

        // This function visits all subsets of O, and applies function
        // exp_getter above to each of them (this function gets the
        // expected cost and sums it to variable total).
        as::combi::visit_subsets(
            std::count(offered.begin(), offered.end(), true), &exp_getter);

        if(dataset_for_csv) {
            dataset_for_csv->add_row_for(offered, total);
        }

        return {total, true};
    }
}