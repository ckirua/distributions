#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct BaldingnicholsDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double f_;
    BaldingnicholsDistribution(double f) : f_(f) {
        detail::assert_strictly_positive(f_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_balding_nichols(rng, f_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Baldingnichols = BaldingnicholsDistribution<double>;

}  // namespace distributions
