#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct BinghamDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double kappa_;
    BinghamDistribution(double kappa) : kappa_(kappa) {
        detail::assert_nonnegative(kappa_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_bingham_x(rng, kappa_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Bingham = BinghamDistribution<double>;

}  // namespace distributions
