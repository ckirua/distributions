#pragma once

#include "distributions/base.hpp"
#include "distributions/detail/math.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>

namespace distributions {

struct Geometric : DistributionBase<Geometric, int, Pcg32> {
    double p;

    explicit Geometric(double p = 0.5) : p(p) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        const double u = rng.next_double();
        return static_cast<int>(std::floor(std::log1p(-u) / detail::log1p_neg(p))) + 1;
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 1.0 / p; }

    [[nodiscard]] double variance() const { return (1.0 - p) / (p * p); }
};

}  // namespace distributions
