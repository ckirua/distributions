#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct CantorDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double loc_;
    double scale_;
    CantorDistribution(double loc, double scale) : loc_(loc), scale_(scale) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_cantor(rng, loc_, scale_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Cantor = CantorDistribution<double>;

}  // namespace distributions
