#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Yulesimon {
    double rho_;
    Yulesimon(double rho) : rho_(rho) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_yule_simon(rng, rho_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
