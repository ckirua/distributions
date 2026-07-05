#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct LkjDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double eta_;
    LkjDistribution(double eta) : eta_(eta) {
        detail::assert_finite(eta_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_lkj_rho(rng, eta_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Lkj = LkjDistribution<double>;

}  // namespace distributions
