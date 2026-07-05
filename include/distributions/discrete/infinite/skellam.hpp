#pragma once

#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/skellam.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct SkellamDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double mu1;
    double mu2;

    SkellamDistribution(double mu1 = 1.0, double mu2 = 1.0) : mu1(mu1), mu2(mu2) {}

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(
            detail::sample_poisson(rng, mu1) - detail::sample_poisson(rng, mu2));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::skellam_sample_batch(
                    reinterpret_cast<int*>(out), n, mu1, mu2, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return mu1 - mu2; }

    [[nodiscard]] double variance() const { return mu1 + mu2; }
};

using Skellam = SkellamDistribution<int>;

}  // namespace distributions
