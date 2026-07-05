#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate_discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MultinomialDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    int n_;
    double p0_;
    MultinomialDistribution(int n, double p0) : n_(n), p0_(p0) {
        detail::assert_nonnegative_int(n_);
        detail::assert_finite(p0_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_multinomial_first(rng, n_, p0_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Multinomial = MultinomialDistribution<double>;

}  // namespace distributions
