#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/hypergeometric.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct HypergeometricDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int M_;
    int n_success_;
    int N_draws_;
    HypergeometricDistribution(int M, int n_success, int N_draws) : M_(M), n_success_(n_success), N_draws_(N_draws) {
        detail::assert_nonnegative_int(M_);
        detail::assert_nonnegative_int(n_success_);
        detail::assert_nonnegative_int(N_draws_);
        assert(n_success_ <= M_);
        assert(N_draws_ <= M_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_hypergeometric(rng, M_, n_success_, N_draws_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Hypergeometric = HypergeometricDistribution<int>;

}  // namespace distributions
