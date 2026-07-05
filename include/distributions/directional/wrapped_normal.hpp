#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/circular.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct WrappedNormalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double mu_;
    double sigma_;
    WrappedNormalDistribution(double mu, double sigma) : mu_(mu), sigma_(sigma) {
        detail::assert_finite(mu_);
        detail::assert_strictly_positive(sigma_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::wrap_angle(detail::sample_normal(rng, mu_, sigma_)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using WrappedNormal = WrappedNormalDistribution<double>;

}  // namespace distributions
