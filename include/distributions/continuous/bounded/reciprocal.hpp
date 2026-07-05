#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ReciprocalDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    double b_;
    ReciprocalDistribution(double a, double b) : a_(a), b_(b) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
        detail::assert_double_interval(a_, b_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(std::exp(detail::sample_uniform(rng, std::log(a_), std::log(b_))));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Reciprocal = ReciprocalDistribution<double>;

}  // namespace distributions
