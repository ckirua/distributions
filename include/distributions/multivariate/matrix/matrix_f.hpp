#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MatrixF {
    double df1_;
    double df2_;
    double v00_;
    double v11_;
    MatrixF(double df1, double df2, double v00, double v11) : df1_(df1), df2_(df2), v00_(v00), v11_(v11) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_matrix_f_trace(rng, df1_, df2_, v00_, v11_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
