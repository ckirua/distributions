#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Complex {
    double df_;
    double v00_;
    double v11_;
    Complex(double df, double v00, double v11) : df_(df), v00_(v00), v11_(v11) {
        detail::assert_strictly_positive(df_);
        detail::assert_finite(v00_);
        detail::assert_finite(v11_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_complex_wishart_trace(rng, df_, v00_, v11_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
