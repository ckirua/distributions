#pragma once

#include "distributions/base.hpp"
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/bernoulli.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/simd/bernoulli.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct BernoulliDistribution : DistributionBase<BernoulliDistribution<Sample>, Sample, Pcg32> {
    static_assert(is_discrete_sample_v<Sample>);

    double p;

    explicit BernoulliDistribution(double p = 0.5) : p(p) {}

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(rng.next_double() < p ? 1 : 0);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, int>) {
            if (n >= detail::kFastThreshold) {
                detail::simd::bernoulli_sample_batch(out, n, p, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return p; }

    [[nodiscard]] double variance() const { return p * (1.0 - p); }
};

/// Default hand-written Bernoulli (``int`` samples; Tier B/C when ``n >= kFastThreshold``).
using Bernoulli = BernoulliDistribution<int>;

}  // namespace distributions
