#pragma once

#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/poisson_binomial.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <type_traits>
#include <vector>

namespace distributions {

template <typename Sample = int>
struct PoissonBinomialDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    std::vector<double> probs;

    explicit PoissonBinomialDistribution(std::vector<double> probs) : probs(std::move(probs)) {}

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        int sum = 0;
        for (double p : probs) {
            if (rng.next_double() < p) {
                ++sum;
            }
        }
        return static_cast<Sample>(sum);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::poisson_binomial_sample_batch(
                    reinterpret_cast<int*>(out),
                    n,
                    probs.data(),
                    probs.size(),
                    detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const {
        double m = 0.0;
        for (double p : probs) {
            m += p;
        }
        return m;
    }

    [[nodiscard]] double variance() const {
        double v = 0.0;
        for (double p : probs) {
            v += p * (1.0 - p);
        }
        return v;
    }
};

using PoissonBinomial = PoissonBinomialDistribution<int>;

}  // namespace distributions
