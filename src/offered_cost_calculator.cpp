#include "offered_cost_calculator.h"

namespace stok {
    OfferedCostCalculator::OfferedCostCalculator(const Params& p) :
        params{p}, acc{p}
    {
        if(params.dataset_file) {
            dataset_for_csv.emplace(params);
        }
    }

    OfferedCostCalculator::~OfferedCostCalculator() {
        if(dataset_for_csv) {
            assert(params.dataset_file);
            for(const auto& row : dataset_for_csv->c) {
                std::ostringstream csv_row;
                std::copy(row.begin(), row.end(),
                    std::ostream_iterator<float>(csv_row, ","));

                *(params.dataset_file) <<
                    csv_row.str().substr(0, csv_row.str().size() - 1u) << "\n";
            }
        }
    }
}