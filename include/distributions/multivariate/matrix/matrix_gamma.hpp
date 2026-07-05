#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MatrixGamma {
    double shape_;
    double v00_;
    double v11_;
    MatrixGamma(double shape, double v00, double v11) : shape_(shape), v00_(v00), v11_(v11) {
        detail::assert_strictly_positive(shape_);
        detail::assert_finite(v00_);
        detail::assert_finite(v11_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_matrix_gamma_trace(rng, shape_, v00_, v11_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
