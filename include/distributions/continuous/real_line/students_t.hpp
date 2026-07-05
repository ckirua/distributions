#pragma once

#include "distributions/detail/student_t.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct StudentT {
    double df_;
    StudentT(double df) : df_(df) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_student_t(rng, df_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
