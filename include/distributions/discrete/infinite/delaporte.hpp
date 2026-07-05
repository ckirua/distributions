#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct DelaporteDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double lambda_;
    double r_;
    double p_;
    DelaporteDistribution(double lambda, double r, double p) : lambda_(lambda), r_(r), p_(p) {
        detail::assert_nonnegative(lambda_);
        detail::assert_strictly_positive(r_);
        detail::assert_probability(p_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_delaporte(rng, lambda_, r_, p_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Delaporte = DelaporteDistribution<int>;

}  // namespace distributions
