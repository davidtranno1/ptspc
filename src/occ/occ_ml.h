#ifndef STOK_OCC_ML
#define STOK_OCC_ML

#include "../offered_cost_calculator.h"
#include "../customer_set.h"
#include "../params.h"
#include "occ_exact.h"

#include <Python.h>
#include <boost/python.hpp>

#include <chrono>
#include <memory>

namespace stok {
    enum class MLModel {
        RegressionTree,
        LinearRegressionTree
    };

    struct OCCML : public OfferedCostCalculator {
        OCCExact occ_exact; // use it for the training dataset.

        mutable boost::python::object main_module;
        mutable boost::python::object main_ns;
        mutable bool model_trained = false;
        mutable float training_mean = 0.0f;
        mutable float training_std = 0.0f;

        // This needs to be a pointer because otherwise it gets initialised
        // on object creation (i.e., d = PyDataset{}). In turn, this initialises
        // the inner contained of d (i.e., d.c = py::list{}). But the messy
        // Python API forbids to do that before calling the global
        // Py_Initialize() function, which can only happen in the constructor
        // body!
        mutable std::unique_ptr<PyDataset> d;
        
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        const float clip_value = 3.0f;

        explicit OCCML(const Params& p);

        OfferedCost operator()(const CustomerSet& offered) const override;
        ~OCCML() override = default;

        virtual void train_model() const = 0;
        virtual float get_prediction(
            const boost::python::list& row) const = 0;

        float predict(const boost::python::list& row) const;
    };
}

#endif