#pragma once

#include "distributions/detail/circular.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct WrappedNormal {
    double mu_;
    double sigma_;
    WrappedNormal(double mu, double sigma) : mu_(mu), sigma_(sigma) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::wrap_angle(detail::sample_normal(rng, mu_, sigma_));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
