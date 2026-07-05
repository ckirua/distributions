#pragma once

#include <cmath>
#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct NakagamiDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double nu_;
    double scale_;
    NakagamiDistribution(double nu, double scale) : nu_(nu), scale_(scale) {
        detail::assert_strictly_positive(nu_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(scale_ * std::sqrt(detail::sample_gamma(rng, nu_, 1.0)));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Nakagami = NakagamiDistribution<double>;

}  // namespace distributions
