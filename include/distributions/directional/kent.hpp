#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct KentDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double kappa_;
    double beta_;
    KentDistribution(double kappa, double beta) : kappa_(kappa), beta_(beta) {
        detail::assert_nonnegative(kappa_);
        detail::assert_strictly_positive(beta_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_kent_x(rng, kappa_, beta_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Kent = KentDistribution<double>;

}  // namespace distributions
