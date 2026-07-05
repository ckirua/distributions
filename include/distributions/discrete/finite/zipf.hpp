#pragma once

#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/zipf.hpp"
#include "distributions/rng.hpp"

#include <cmath>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace distributions {

template <typename Sample = int>
struct ZipfDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int n;
    double s;

    ZipfDistribution(int n = 10, double s = 2.0) : n(n), s(s) { rebuild(); }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = rng.next_double() * cdf_n_;
        int lo = 1;
        int hi = n;
        while (lo < hi) {
            const int mid = (lo + hi) / 2;
            if (cdf_[static_cast<std::size_t>(mid)] < u) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return static_cast<Sample>(lo);
    }

    void sample_batch(Sample* out, std::size_t n_out, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n_out >= detail::kFastThreshold) {
                detail::fast::zipf_sample_batch(
                    reinterpret_cast<int*>(out),
                    n_out,
                    n,
                    cdf_.data(),
                    cdf_n_,
                    detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n_out; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 0.0; }

    [[nodiscard]] double variance() const { return 0.0; }

private:
    std::vector<double> cdf_;
    double cdf_n_{0.0};

    void rebuild() {
        cdf_.assign(static_cast<std::size_t>(n) + 1, 0.0);
        double sum = 0.0;
        for (int k = 1; k <= n; ++k) {
            sum += 1.0 / std::pow(static_cast<double>(k), s);
            cdf_[static_cast<std::size_t>(k)] = sum;
        }
        cdf_n_ = sum;
        for (int k = 1; k <= n; ++k) {
            cdf_[static_cast<std::size_t>(k)] /= sum;
        }
    }
};

template <typename Sample = int>
struct ZipfMandelbrotDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int n;
    double q;
    double s;

    ZipfMandelbrotDistribution(int n = 10, double q = 1.0, double s = 2.0) : n(n), q(q), s(s) {
        rebuild();
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        const double u = rng.next_double() * cdf_n_;
        int lo = 1;
        int hi = n;
        while (lo < hi) {
            const int mid = (lo + hi) / 2;
            if (cdf_[static_cast<std::size_t>(mid)] < u) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return static_cast<Sample>(lo);
    }

    void sample_batch(Sample* out, std::size_t n_out, Pcg32& rng) const {
        if constexpr (is_discrete_sample_v<Sample>) {
            if (n_out >= detail::kFastThreshold) {
                detail::fast::zipf_sample_batch(
                    reinterpret_cast<int*>(out),
                    n_out,
                    n,
                    cdf_.data(),
                    cdf_n_,
                    detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n_out; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 0.0; }

    [[nodiscard]] double variance() const { return 0.0; }

private:
    std::vector<double> cdf_;
    double cdf_n_{0.0};

    void rebuild() {
        cdf_.assign(static_cast<std::size_t>(n) + 1, 0.0);
        double sum = 0.0;
        for (int k = 1; k <= n; ++k) {
            sum += 1.0 / std::pow(static_cast<double>(k) + q, s);
            cdf_[static_cast<std::size_t>(k)] = sum;
        }
        cdf_n_ = sum;
        for (int k = 1; k <= n; ++k) {
            cdf_[static_cast<std::size_t>(k)] /= sum;
        }
    }
};

using Zipf = ZipfDistribution<int>;
using ZipfMandelbrot = ZipfMandelbrotDistribution<int>;

}  // namespace distributions
