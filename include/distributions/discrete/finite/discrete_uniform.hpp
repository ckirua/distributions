#pragma once

#include "distributions/base.hpp"
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/simd/discrete_uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct DiscreteUniformDistribution
    : DistributionBase<DiscreteUniformDistribution<Sample>, Sample, Pcg32> {
    static_assert(is_discrete_sample_v<Sample>);

    int low;
    int high;

    DiscreteUniformDistribution(int low = 0, int high = 1) : low(low), high(high) {
        detail::assert_int_interval(low, high);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const int span = high - low + 1;
        return static_cast<Sample>(
            low + static_cast<int>(rng.next_u32() % static_cast<std::uint32_t>(span)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n >= detail::kFastThreshold) {
                detail::simd::discrete_uniform_sample_batch(
                    reinterpret_cast<int*>(out), n, low, high, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 0.5 * (static_cast<double>(low) + high); }

    [[nodiscard]] double variance() const {
        const double span = static_cast<double>(high - low + 1);
        return (span * span - 1.0) / 12.0;
    }
};

/// Default hand-written discrete uniform (``int`` samples; Tier B/C when ``n >= kFastThreshold``).
using DiscreteUniform = DiscreteUniformDistribution<int>;

}  // namespace distributions
