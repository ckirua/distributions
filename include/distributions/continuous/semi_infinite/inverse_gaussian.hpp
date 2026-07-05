#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/inverse_gaussian.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct InverseGaussianDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double mu_;
    double lambda_;
    InverseGaussianDistribution(double mu, double lambda) : mu_(mu), lambda_(lambda) {
        detail::assert_nonnegative(mu_);
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_inverse_gaussian(rng, mu_, lambda_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using InverseGaussian = InverseGaussianDistribution<double>;

}  // namespace distributions
