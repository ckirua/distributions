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
struct KumaraswamyDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    double b_;
    KumaraswamyDistribution(double a, double b) : a_(a), b_(b) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = detail::sample_uniform(rng, 0.0, 1.0);
                const double v = detail::sample_uniform(rng, 0.0, 1.0);
                return static_cast<Sample>(std::pow(1.0 - std::pow(1.0 - u, 1.0 / a_), 1.0 / b_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Kumaraswamy = KumaraswamyDistribution<double>;

}  // namespace distributions
