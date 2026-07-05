#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/log_normal.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct LogNormalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double mu_;
    double sigma_;
    LogNormalDistribution(double mu, double sigma) : mu_(mu), sigma_(sigma) {
        detail::assert_finite(mu_);
        detail::assert_strictly_positive(sigma_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(std::exp(detail::sample_normal(rng, mu_, sigma_)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::log_normal_sample_batch(out, n, mu_, sigma_, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using LogNormal = LogNormalDistribution<double>;

}  // namespace distributions
