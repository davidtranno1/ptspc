#include "occ_ml_rtlr.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace stok {
    namespace {
        namespace py = boost::python;
    }

    void OCCMLRTLR::train_model() const {
        using namespace boost::filesystem;

        assert(py::len(d->container()) > 0);

        // Add the cwd to Python's path, so it finds tree_utils.py
        auto cwd = absolute("./").normalize();
        PyObject* system_path = PySys_GetObject("path");
        PyList_Insert(system_path, 0, PyUnicode_FromString(
            cwd.string().c_str()));
        // --- --- --- --- --- --- --- --- --- --- --- --- --- ---

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
                "from sklearn.pipeline import make_pipeline\n"
                "from tree_utils import TreeWithRegression\n"
                "import numpy as np\n"
                "X = [row[:-1] for row in data]\n"
                "y = [row[-1] for row in data]\n"
                "ym, ys = np.mean(y), np.std(y)\n"
                "m = make_pipeline(StandardScaler(), TreeWithRegression(max_depth=5))\n"
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

    float OCCMLRTLR::get_prediction(const boost::python::list& row) const {
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
                "prediction = m.predict([input])[0]\n"
                "if type(prediction) is np.ndarray:\n"
                "    prediction = prediction[0]\n",
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