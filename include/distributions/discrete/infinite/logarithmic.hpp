#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/discrete.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct LogarithmicDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double p_;
    LogarithmicDistribution(double p) : p_(p) {
        detail::assert_open_unit_interval(p_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_logarithmic(rng, p_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Logarithmic = LogarithmicDistribution<int>;

}  // namespace distributions
