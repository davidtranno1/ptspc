#include "occ_ml_rt.h"

#include <iostream>

namespace stok {
    namespace {
        namespace py = boost::python;
    }

    void OCCMLRT::train_model() const {
        assert(py::len(d->container()) > 0);

        try {
            main_ns["data"] = d->container();
        } catch(const py::error_already_set&) {
            std::cerr << "Error setting data\n";
            PyErr_Print();
            throw;
        }

        try {
            py::exec(
                "from sklearn.preprocessing import StandardScaler\n"
                "from sklearn.tree import DecisionTreeRegressor\n"
                "from sklearn.pipeline import make_pipeline\n"
                "import numpy as np\n"
                "X = [row[:-1] for row in data]\n"
                "y = [row[-1] for row in data]\n"
                "ym, ys = np.mean(y), np.std(y)\n"
                "m = make_pipeline(StandardScaler(), DecisionTreeRegressor(max_depth=7))\n"
                "m.fit(X, y)\n",
                main_ns
            );
        } catch(const py::error_already_set&) {
            std::cerr << "Error training model\n";
            PyErr_Print();
            throw;
        }

        try {
            training_mean = py::extract<float>(main_ns["ym"]);
            training_std = py::extract<float>(main_ns["ys"]);
        } catch(const py::error_already_set&) {
            std::cerr << "Error getting mean and std\n";
            PyErr_Print();
            throw;
        }
    }

    float OCCMLRT::get_prediction(const boost::python::list& row) const {
        assert(model_trained);

        try {
            main_ns["input"] = row;
        } catch(const py::error_already_set&) {
            std::cerr << "Error setting input\n";
            PyErr_Print();
            throw;
        }

        try {
            py::exec(
                "prediction = m.predict([input])[0]",
                main_ns
            );
        } catch(const py::error_already_set&) {
            std::cerr << "Error getting prediction\n";
            PyErr_Print();
            throw;
        }

        try {
            return py::extract<float>(main_ns["prediction"]);
        } catch(const py::error_already_set&) {
            std::cerr << "Error extracting prediction\n";
            PyErr_Print();
            throw;
        }
    }
}