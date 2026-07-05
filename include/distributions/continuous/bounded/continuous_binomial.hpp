#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ContinuousBinomialDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    int n_;
    ContinuousBinomialDistribution(int n) : n_(n) {
        detail::assert_nonnegative_int(n_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_continuous_binomial(rng, n_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using ContinuousBinomial = ContinuousBinomialDistribution<double>;

}  // namespace distributions
