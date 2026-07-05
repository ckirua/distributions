#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct UQuadraticDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    double b_;
    UQuadraticDistribution(double a, double b) : a_(a), b_(b) {
        detail::assert_double_interval(a_, b_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
                const double t = a_ + (b_ - a_) * u;
                return static_cast<Sample>(0.5 * (a_ + b_) + 0.5 * (b_ - a_) * (2.0 * t / (a_ + b_) - 1.0));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using UQuadratic = UQuadraticDistribution<double>;

}  // namespace distributions
