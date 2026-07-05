#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate_discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct EwensDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double theta_;
    int n_;
    EwensDistribution(double theta, int n) : theta_(theta), n_(n) {
        detail::assert_finite(theta_);
        detail::assert_nonnegative_int(n_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_ewens_first(rng, theta_, n_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Ewens = EwensDistribution<double>;

}  // namespace distributions
