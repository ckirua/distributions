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
struct RiceDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double b_;
    double scale_;
    RiceDistribution(double b, double scale) : b_(b), scale_(scale) {
        detail::assert_strictly_positive(b_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double z1 = detail::sample_standard_normal(rng);
                const double z2 = detail::sample_standard_normal(rng);
                return static_cast<Sample>(scale_ * std::sqrt(std::pow(b_ + z1, 2) + z2 * z2));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Rice = RiceDistribution<double>;

}  // namespace distributions
