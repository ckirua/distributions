#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeneralizedNormal {
    double beta_;
    double loc_;
    double scale_;
    GeneralizedNormal(double beta, double loc, double scale) : beta_(beta), loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_gennorm(rng, beta_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
