#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct PolyWeibull {
    double shape1_;
    double shape2_;
    double weight_;
    PolyWeibull(double shape1, double shape2, double weight) : shape1_(shape1), shape2_(shape2), weight_(weight) {
        detail::assert_finite(shape1_);
        detail::assert_finite(shape2_);
        detail::assert_probability(weight_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_poly_weibull(rng, shape1_, shape2_, weight_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
