#pragma once

#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/exponential.hpp"
#include "distributions/detail/simd/exponential.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ExponentialDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double rate;

    explicit ExponentialDistribution(double rate = 1.0) : rate(rate) {}

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        using Real = compute_type_t<Sample>;
        const Real u = static_cast<Real>(rng.next_double());
        const Real x = -std::log1p(-u) / static_cast<Real>(rate);
        return static_cast<Sample>(x);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::simd::exponential_sample_batch(out, n, rate, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 1.0 / rate; }

    [[nodiscard]] double variance() const { return 1.0 / (rate * rate); }
};

/// Default hand-written exponential (``double`` samples; Tier B/C when ``n >= kFastThreshold``).
using Exponential = ExponentialDistribution<double>;

}  // namespace distributions
