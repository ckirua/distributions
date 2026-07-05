#pragma once

#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/negative_binomial.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct NegativeBinomialDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int r;
    double p;

    NegativeBinomialDistribution(int r = 1, double p = 0.5) : r(r), p(p) {
        detail::assert_positive_int(r);
        detail::assert_open_unit_interval(p);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double lambda = detail::sample_gamma(rng, static_cast<double>(r), 1.0);
        const double rate = (1.0 - p) / p;
        return static_cast<Sample>(detail::sample_poisson(rng, lambda * rate));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::negative_binomial_sample_batch(
                    reinterpret_cast<int*>(out), n, r, p, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return static_cast<double>(r) * (1.0 - p) / p; }

    [[nodiscard]] double variance() const {
        return static_cast<double>(r) * (1.0 - p) / (p * p);
    }
};

using NegativeBinomial = NegativeBinomialDistribution<int>;

}  // namespace distributions
