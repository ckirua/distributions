#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct HotellingsTSquaredDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double df1_;
    double df2_;
    HotellingsTSquaredDistribution(double df1, double df2) : df1_(df1), df2_(df2) {
        detail::assert_finite(df1_);
        detail::assert_finite(df2_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_hotelling_t_squared(rng, df1_, df2_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using HotellingsTSquared = HotellingsTSquaredDistribution<double>;

}  // namespace distributions
