#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeometricStable {
    double alpha_;
    double beta_;
    double loc_;
    double scale_;
    GeometricStable(double alpha, double beta, double loc, double scale) : alpha_(alpha), beta_(beta), loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_stable(rng, alpha_, beta_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
