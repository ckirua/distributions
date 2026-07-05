#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/multivariate.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct GeneralizedDirichletDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a1_;
    double b1_;
    double a2_;
    double b2_;
    GeneralizedDirichletDistribution(double a1, double b1, double a2, double b2) : a1_(a1), b1_(b1), a2_(a2), b2_(b2) {
        detail::assert_finite(a1_);
        detail::assert_finite(b1_);
        detail::assert_finite(a2_);
        detail::assert_finite(b2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_generalized_dirichlet_first(rng, a1_, b1_, a2_, b2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using GeneralizedDirichlet = GeneralizedDirichletDistribution<double>;

}  // namespace distributions
