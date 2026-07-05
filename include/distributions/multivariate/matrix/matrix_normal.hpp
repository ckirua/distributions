#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MatrixNormalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double row_var_;
    double col_var_;
    MatrixNormalDistribution(double row_var, double col_var) : row_var_(row_var), col_var_(col_var) {
        detail::assert_finite(row_var_);
        detail::assert_finite(col_var_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_matrix_normal_elem(rng, row_var_, col_var_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MatrixNormal = MatrixNormalDistribution<double>;

}  // namespace distributions
