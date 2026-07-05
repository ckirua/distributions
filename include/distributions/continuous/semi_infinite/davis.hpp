#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct DavisDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double nu_;
    DavisDistribution(double nu) : nu_(nu) {
        detail::assert_strictly_positive(nu_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_davis(rng, nu_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Davis = DavisDistribution<double>;

}  // namespace distributions
