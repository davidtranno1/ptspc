#ifndef STOK_OCC_ML_RTLR
#define STOK_OCC_ML_RTLR

#include "occ_ml.h"
#include "../params.h"

namespace stok {
    struct OCCMLRTLR : public OCCML {
        explicit OCCMLRTLR(const Params& p) : OCCML{p} {}

        void train_model() const override;
        float get_prediction(const boost::python::list& row) const override;
    };
}

#endif