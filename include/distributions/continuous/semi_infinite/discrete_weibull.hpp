#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/discrete_weibull.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct DiscreteWeibullDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double c_;
    DiscreteWeibullDistribution(double c) : c_(c) {
        detail::assert_strictly_positive(c_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_discrete_weibull(rng, c_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::discrete_weibull_sample_batch(out, n, c_, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using DiscreteWeibull = DiscreteWeibullDistribution<double>;

}  // namespace distributions
