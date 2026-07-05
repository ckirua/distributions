#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct ConwaymaxwellpoissonDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double lambda_;
    double nu_;
    ConwaymaxwellpoissonDistribution(double lambda, double nu) : lambda_(lambda), nu_(nu) {
        detail::assert_nonnegative(lambda_);
        detail::assert_strictly_positive(nu_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_comp_poisson(rng, lambda_, nu_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Conwaymaxwellpoisson = ConwaymaxwellpoissonDistribution<int>;

}  // namespace distributions
