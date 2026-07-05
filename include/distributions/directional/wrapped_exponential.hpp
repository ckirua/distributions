#pragma once

#include "distributions/detail/circular.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct WrappedExponential {
    double loc_;
    double rate_;
    WrappedExponential(double loc, double rate) : loc_(loc), rate_(rate) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(rate_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = rng.next_double();
        return detail::wrap_angle(loc_ - std::log1p(-u) / rate_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
