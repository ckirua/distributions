#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate_discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct DirichletMultinomialDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    int n_;
    double a0_;
    double a1_;
    double a2_;
    DirichletMultinomialDistribution(int n, double a0, double a1, double a2) : n_(n), a0_(a0), a1_(a1), a2_(a2) {
        detail::assert_nonnegative_int(n_);
        detail::assert_finite(a0_);
        detail::assert_finite(a1_);
        detail::assert_finite(a2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_dirichlet_multinomial_first(rng, n_, a0_, a1_, a2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using DirichletMultinomial = DirichletMultinomialDistribution<double>;

}  // namespace distributions
