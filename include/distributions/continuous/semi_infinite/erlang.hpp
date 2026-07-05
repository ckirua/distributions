#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Erlang {
    double a_;
    double scale_;
    Erlang(double a, double scale) : a_(a), scale_(scale) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_gamma(rng, a_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
