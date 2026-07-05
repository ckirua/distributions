#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct HyperDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double lambda1_;
    double lambda2_;
    double w1_;
    HyperDistribution(double lambda1, double lambda2, double w1) : lambda1_(lambda1), lambda2_(lambda2), w1_(w1) {
        detail::assert_nonnegative(lambda1_);
        detail::assert_nonnegative(lambda2_);
        detail::assert_probability(w1_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_hyper_mixture(rng, lambda1_, lambda2_, w1_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Hyper = HyperDistribution<double>;

}  // namespace distributions
