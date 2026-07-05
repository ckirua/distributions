#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct WilkssLambda {
    double a_;
    double b_;
    WilkssLambda(double a, double b) : a_(a), b_(b) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_wilks_lambda(rng, a_, b_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
