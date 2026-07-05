#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/hypergeometric.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct NegativeHypergeometricDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int M_;
    int n_;
    int N_;
    NegativeHypergeometricDistribution(int M, int n, int N) : M_(M), n_(n), N_(N) {
        detail::assert_nonnegative_int(M_);
        detail::assert_nonnegative_int(n_);
        detail::assert_nonnegative_int(N_);
        assert(n_ <= M_);
        assert(N_ <= M_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const int k = detail::sample_hypergeometric(rng, M_, n_, N_);
                return static_cast<Sample>(detail::sample_negative_binomial(rng, static_cast<double>(k), static_cast<double>(N_) / static_cast<double>(M_)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using NegativeHypergeometric = NegativeHypergeometricDistribution<int>;

}  // namespace distributions
