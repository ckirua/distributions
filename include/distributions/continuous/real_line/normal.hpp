#pragma once

#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/normal.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>
#include <numbers>

namespace distributions {

struct Normal {
    double mu;
    double sigma;

    Normal(double mu = 0.0, double sigma = 1.0) : mu(mu), sigma(sigma) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        const double u1 = std::max(rng.next_double(), 1e-300);
        const double u2 = rng.next_double();
        const double z =
            std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * std::numbers::pi * u2);
        return mu + sigma * z;
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        if (n >= detail::kFastThreshold) {
            detail::fast::normal_sample_batch(out, n, mu, sigma, detail::batch_seed_from(rng));
            return;
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return mu; }

    [[nodiscard]] double variance() const { return sigma * sigma; }
};

}  // namespace distributions
