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
struct ParetoDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double alpha_;
    double scale_;
    ParetoDistribution(double alpha, double scale) : alpha_(alpha), scale_(scale) {
        detail::assert_strictly_positive(alpha_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(scale_ / std::pow(rng.next_double(), 1.0 / alpha_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Pareto = ParetoDistribution<double>;

}  // namespace distributions
