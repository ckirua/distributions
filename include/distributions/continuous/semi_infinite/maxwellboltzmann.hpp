#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/normal.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MaxwellBoltzmannDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    MaxwellBoltzmannDistribution(double a) : a_(a) {
        detail::assert_strictly_positive(a_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double x = detail::sample_standard_normal(rng);
                const double y = detail::sample_standard_normal(rng);
                const double z = detail::sample_standard_normal(rng);
                return static_cast<Sample>(a_ * std::sqrt(x * x + y * y + z * z));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MaxwellBoltzmann = MaxwellBoltzmannDistribution<double>;

}  // namespace distributions
