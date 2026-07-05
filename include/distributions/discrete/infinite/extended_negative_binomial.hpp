#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct ExtendedNegativeBinomialDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double r_;
    double p_;
    ExtendedNegativeBinomialDistribution(double r, double p) : r_(r), p_(p) {
        detail::assert_strictly_positive(r_);
        detail::assert_open_unit_interval(p_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_negative_binomial_gamma_poisson(rng, r_, p_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using ExtendedNegativeBinomial = ExtendedNegativeBinomialDistribution<int>;

}  // namespace distributions
