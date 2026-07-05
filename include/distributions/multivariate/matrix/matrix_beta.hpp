#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MatrixBetaDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    double b_;
    MatrixBetaDistribution(double a, double b) : a_(a), b_(b) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_matrix_beta_trace(rng, a_, b_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MatrixBeta = MatrixBetaDistribution<double>;

}  // namespace distributions
