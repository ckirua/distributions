#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MatrixBeta {
    double a_;
    double b_;
    MatrixBeta(double a, double b) : a_(a), b_(b) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_matrix_beta_trace(rng, a_, b_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
