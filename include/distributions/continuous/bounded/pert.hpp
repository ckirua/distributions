#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct PertDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double lo_;
    double mode_;
    double hi_;
    PertDistribution(double lo, double mode, double hi) : lo_(lo), mode_(mode), hi_(hi) {
        detail::assert_finite(mode_);
        detail::assert_double_interval(lo_, hi_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_pert(rng, lo_, mode_, hi_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Pert = PertDistribution<double>;

}  // namespace distributions
