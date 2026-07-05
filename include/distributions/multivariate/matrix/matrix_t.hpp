#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct MatrixT {
    double df_;
    double row_var_;
    double col_var_;
    MatrixT(double df, double row_var, double col_var) : df_(df), row_var_(row_var), col_var_(col_var) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_matrix_t_elem(rng, df_, row_var_, col_var_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
