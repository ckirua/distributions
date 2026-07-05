#pragma once

#include "distributions/base.hpp"
#include "distributions/concepts.hpp"
#include "distributions/detail/math.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct GeometricDistribution : DistributionBase<GeometricDistribution<Sample>, Sample, Pcg32> {
    static_assert(is_discrete_sample_v<Sample>);

    double p;

    explicit GeometricDistribution(double p = 0.5) : p(p) {}

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = rng.next_double();
        return static_cast<Sample>(
            static_cast<int>(std::floor(std::log1p(-u) / detail::log1p_neg(p))) + 1);
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 1.0 / p; }

    [[nodiscard]] double variance() const { return (1.0 - p) / (p * p); }
};

using Geometric = GeometricDistribution<int>;

}  // namespace distributions
