#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct BorelDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double mu_;
    BorelDistribution(double mu) : mu_(mu) {
        detail::assert_nonnegative(mu_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_borel(rng, mu_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Borel = BorelDistribution<int>;

}  // namespace distributions
