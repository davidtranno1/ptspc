#include "occ_ml.h"

namespace stok {
    namespace {
        namespace py = boost::python;
    }

    OCCML::OCCML(const Params& p) :
        OfferedCostCalculator{p}, occ_exact{p}
    {
        Py_Initialize();
        main_module = py::import("__main__");
        main_ns = main_module.attr("__dict__");
        d = std::make_unique<PyDataset>(p);

        start_time = std::chrono::high_resolution_clock::now();
    }

    OfferedCost OCCML::operator()(const CustomerSet& offered) const {
        using namespace std::chrono;

        const auto n_offered = std::count(offered.begin(), offered.end(), true);

        if( n_offered == 0) {
            return occ_exact(offered);
        }

        if(!model_trained) {
            const auto current_time = high_resolution_clock::now();
            const auto elapsed = duration_cast<duration<double>>(
                current_time - start_time).count();

            if(elapsed <= params.ml_training_time_s) {
                const auto off_cost = occ_exact(offered);
                d->add_row_for(offered, off_cost.cost);

                return off_cost;
            }

            train_model();
            model_trained = true;

            return OfferedCost{
                /*.cost = */predict(d->row_for(offered).second),
                /*.is_exact = */false
            };
        }

        return OfferedCost{
            /*.cost = */predict(d->row_for(offered).second),
            /*.is_exact = */false
        };
    }

    float OCCML::predict(const boost::python::list& row) const {
        assert(training_mean > 0.0f);
        assert(training_std > 0.0f);

        auto prediction = get_prediction(row);
        
        prediction = std::max(prediction,
            training_mean - clip_value * training_std);

        prediction = std::min(prediction,
            training_mean + clip_value * training_std);

        return prediction;
    }
}