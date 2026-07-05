#pragma once

#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/normal.hpp"
#include "distributions/detail/simd/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>
#include <numbers>
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct NormalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double mu;
    double sigma;

    NormalDistribution(double mu = 0.0, double sigma = 1.0) : mu(mu), sigma(sigma) {
        detail::assert_finite(mu);
        detail::assert_strictly_positive(sigma);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        using Real = compute_type_t<Sample>;
        static constexpr Real kTwoPi = Real{6.283185307179586};
        const Real u1 = std::max(static_cast<Real>(rng.next_double()), Real{1e-300});
        const Real u2 = static_cast<Real>(rng.next_double());
        const Real z = std::sqrt(-Real{2.0} * std::log(u1)) * std::cos(kTwoPi * u2);
        return static_cast<Sample>(static_cast<Real>(mu) + static_cast<Real>(sigma) * z);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::simd::normal_sample_batch(out, n, mu, sigma, detail::batch_seed_from(rng));
                return;
            }
        } else if constexpr (std::is_same_v<Sample, float>) {
            if (n >= detail::kFastThreshold) {
                detail::simd::normal_sample_batch(out, n, mu, sigma, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return mu; }

    [[nodiscard]] double variance() const { return sigma * sigma; }
};

/// Default hand-written normal (``double`` samples; Tier B/C when ``n >= kFastThreshold``).
/// ``NormalDistribution<float>`` uses Tier B/C (scalar or 8-wide AVX2) at ``kFastThreshold``.
using Normal = NormalDistribution<double>;

}  // namespace distributions
