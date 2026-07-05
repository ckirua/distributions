#pragma once

#include "distributions/base.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/discrete_uniform.hpp"
#include "distributions/rng.hpp"

#include <cstddef>

namespace distributions {

struct DiscreteUniform : DistributionBase<DiscreteUniform, int, Pcg32> {
    int low;
    int high;

    DiscreteUniform(int low = 0, int high = 1) : low(low), high(high) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        const int span = high - low + 1;
        return low + static_cast<int>(rng.next_u32() % static_cast<std::uint32_t>(span));
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        if (n >= detail::kFastThreshold) {
            detail::fast::discrete_uniform_sample_batch(
                out, n, low, high, detail::batch_seed_from(rng));
            return;
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

}  // namespace distributions
