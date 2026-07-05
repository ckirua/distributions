#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct HotellingsTSquared {
    double df1_;
    double df2_;
    HotellingsTSquared(double df1, double df2) : df1_(df1), df2_(df2) {
        detail::assert_finite(df1_);
        detail::assert_finite(df2_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_hotelling_t_squared(rng, df1_, df2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
