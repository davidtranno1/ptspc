#ifndef STOK_OCC_ML_RT
#define STOK_OCC_ML_RT

#include "occ_ml.h"
#include "../params.h"

namespace stok {
    struct OCCMLRT : public OCCML {
        explicit OCCMLRT(const Params& p) : OCCML{p} {}

        void train_model() const override;
        float get_prediction(const boost::python::list& row) const override;
    };
}

#endif