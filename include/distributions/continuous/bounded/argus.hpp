#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct ArgusDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double chi_;
    ArgusDistribution(double chi) : chi_(chi) {
        detail::assert_strictly_positive(chi_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_argus(rng, chi_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Argus = ArgusDistribution<double>;

}  // namespace distributions
