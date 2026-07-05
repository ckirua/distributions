#pragma once

#include "distributions/base.hpp"
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/binomial.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/simd/binomial.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct BinomialDistribution : DistributionBase<BinomialDistribution<Sample>, Sample, Pcg32> {
    static_assert(is_discrete_sample_v<Sample>);

    int n;
    double p;

    BinomialDistribution(int n = 1, double p = 0.5) : n(n), p(p) {
        detail::assert_nonnegative_int(n);
        detail::assert_probability(p);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        double cumulative = 0.0;
        const double q = std::pow(1.0 - p, static_cast<double>(n));
        double prob = q;
        const double u = rng.next_double();
        cumulative = prob;
        if (u <= cumulative) {
            return Sample{0};
        }
        for (int k = 1; k <= n; ++k) {
            prob *= (static_cast<double>(n - k + 1) / k) * (p / (1.0 - p));
            cumulative += prob;
            if (u <= cumulative) {
                return static_cast<Sample>(k);
            }
        }
        return static_cast<Sample>(n);
    }

    void sample_batch(Sample* out, std::size_t n_out, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n_out >= detail::kFastThreshold && n <= detail::fast::kMaxBernoulliSumTrials) {
                detail::simd::binomial_sample_batch(
                    reinterpret_cast<int*>(out), n_out, n, p, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n_out; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return static_cast<double>(n) * p; }

    [[nodiscard]] double variance() const { return static_cast<double>(n) * p * (1.0 - p); }
};

using Binomial = BinomialDistribution<int>;

}  // namespace distributions
