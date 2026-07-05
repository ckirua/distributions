#pragma once

#include "distributions/base.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/bernoulli.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <cstdint>

namespace distributions {

struct Bernoulli : DistributionBase<Bernoulli, int, Pcg32> {
    double p;

    explicit Bernoulli(double p = 0.5) : p(p) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return rng.next_double() < p ? 1 : 0;
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        if (n >= detail::kFastThreshold) {
            detail::fast::bernoulli_sample_batch(out, n, p, detail::batch_seed_from(rng));
            return;
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return p; }

    [[nodiscard]] double variance() const { return p * (1.0 - p); }
};

}  // namespace distributions
