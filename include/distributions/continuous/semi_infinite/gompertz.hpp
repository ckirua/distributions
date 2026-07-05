#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Gompertz {
    double c_;
    double scale_;
    Gompertz(double c, double scale) : c_(c), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u = rng.next_double();
        return (1.0 / c_) * std::log(1.0 + (c_ / scale_) * (-std::log(u)));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
