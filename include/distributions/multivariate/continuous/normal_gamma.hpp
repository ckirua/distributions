#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct NormalGammaDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double shape_;
    double rate_;
    NormalGammaDistribution(double shape, double rate) : shape_(shape), rate_(rate) {
        detail::assert_strictly_positive(shape_);
        detail::assert_strictly_positive(rate_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_normal_gamma_mean(rng, shape_, rate_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using NormalGamma = NormalGammaDistribution<double>;

}  // namespace distributions
