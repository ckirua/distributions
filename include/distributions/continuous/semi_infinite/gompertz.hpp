#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct GompertzDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double c_;
    double scale_;
    GompertzDistribution(double c, double scale) : c_(c), scale_(scale) {
        detail::assert_strictly_positive(c_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = rng.next_double();
                return static_cast<Sample>((1.0 / c_) * std::log(1.0 + (c_ / scale_) * (-std::log(u))));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Gompertz = GompertzDistribution<double>;

}  // namespace distributions
