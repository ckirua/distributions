#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct FishersZ {
    double n_;
    double loc_;
    double scale_;
    FishersZ(double n, double loc, double scale) : n_(n), loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_fisher_z(rng, n_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
