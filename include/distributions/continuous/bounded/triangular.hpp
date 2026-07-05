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
struct TriangularDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double lo_;
    double mode_;
    double hi_;
    TriangularDistribution(double lo, double mode, double hi) : lo_(lo), mode_(mode), hi_(hi) {
        detail::assert_finite(mode_);
        detail::assert_double_interval(lo_, hi_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = rng.next_double();
                const double c = (mode_ - lo_) / (hi_ - lo_);
                if (u < c) { return static_cast<Sample>(lo_ + std::sqrt(u * (hi_ - lo_) * (mode_ - lo_))); }
                return static_cast<Sample>(hi_ - std::sqrt((1.0 - u) * (hi_ - lo_) * (hi_ - mode_)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Triangular = TriangularDistribution<double>;

}  // namespace distributions
