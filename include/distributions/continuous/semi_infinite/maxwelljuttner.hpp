#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Maxwelljuttner {
    double a_;
    Maxwelljuttner(double a) : a_(a) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_maxwell_juttner(rng, a_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
