#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct DirichletDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a0_;
    double a1_;
    double a2_;
    DirichletDistribution(double a0, double a1, double a2) : a0_(a0), a1_(a1), a2_(a2) {
        detail::assert_finite(a0_);
        detail::assert_finite(a1_);
        detail::assert_finite(a2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_dirichlet_first(rng, a0_, a1_, a2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Dirichlet = DirichletDistribution<double>;

}  // namespace distributions
