#pragma once

#include "distributions/detail/validate.hpp"
#include "distributions/detail/variable_support.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct KaniadakisErlang {
    double kappa_;
    double shape_;
    double scale_;
    KaniadakisErlang(double kappa, double shape, double scale) : kappa_(kappa), shape_(shape), scale_(scale) {
        detail::assert_nonnegative(kappa_);
        detail::assert_strictly_positive(shape_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_kaniadakis_erlang(rng, kappa_, shape_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
