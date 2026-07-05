#include "distributions/detail/fast/bernoulli.hpp"
#include "distributions/detail/fast/binomial.hpp"
#include "distributions/detail/fast/beta_binomial.hpp"
#include "distributions/detail/fast/discrete_uniform.hpp"
#include "distributions/detail/fast/exponential.hpp"
#include "distributions/detail/fast/normal.hpp"
#include "distributions/detail/fast/poisson_binomial.hpp"
#include "distributions/detail/simd/bernoulli.hpp"
#include "distributions/detail/simd/binomial.hpp"
#include "distributions/detail/simd/beta_binomial.hpp"
#include "distributions/detail/simd/config.hpp"
#include "distributions/detail/simd/discrete_uniform.hpp"
#include "distributions/detail/simd/exponential.hpp"
#include "distributions/detail/simd/normal.hpp"
#include "distributions/detail/simd/poisson_binomial.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

struct Stats {
    double mean{0.0};
    double var{0.0};
};

Stats compute_stats(const int* data, std::size_t n) {
    double sum = 0.0;
    double sum_sq = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const double x = static_cast<double>(data[i]);
        sum += x;
        sum_sq += x * x;
    }
    const double mean = sum / static_cast<double>(n);
    return Stats{mean, (sum_sq / static_cast<double>(n)) - mean * mean};
}

Stats compute_stats(const double* data, std::size_t n) {
    double sum = 0.0;
    double sum_sq = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        sum += data[i];
        sum_sq += data[i] * data[i];
    }
    const double mean = sum / static_cast<double>(n);
    return Stats{mean, (sum_sq / static_cast<double>(n)) - mean * mean};
}

Stats compute_stats(const float* data, std::size_t n) {
    double sum = 0.0;
    double sum_sq = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const double x = static_cast<double>(data[i]);
        sum += x;
        sum_sq += x * x;
    }
    const double mean = sum / static_cast<double>(n);
    return Stats{mean, (sum_sq / static_cast<double>(n)) - mean * mean};
}

bool check_moments(const Stats& a, const Stats& b, std::size_t n, double alpha) {
    const double z = 3.0;  // ~99.7% for large n
    const double se_mean = std::sqrt(std::max(a.var, b.var) / static_cast<double>(n));
    if (std::abs(a.mean - b.mean) > z * se_mean + 1e-9) {
        std::cerr << "mean mismatch " << a.mean << " vs " << b.mean << '\n';
        return false;
    }
    const double se_var = std::sqrt(2.0 * std::max(a.var, b.var) * std::max(a.var, b.var) /
                                    static_cast<double>(n));
    if (std::abs(a.var - b.var) > z * se_var + 1e-9) {
        std::cerr << "variance mismatch " << a.var << " vs " << b.var << '\n';
        return false;
    }
    (void)alpha;
    return true;
}

bool check_discrete_chi(const int* a, const int* b, std::size_t n, double alpha) {
    std::unordered_map<int, int> count_a;
    std::unordered_map<int, int> count_b;
    for (std::size_t i = 0; i < n; ++i) {
        ++count_a[a[i]];
        ++count_b[b[i]];
    }
    double chi_a = 0.0;
    double chi_b = 0.0;
    for (const auto& [key, ca] : count_a) {
        const int cb = count_b.count(key) ? count_b.at(key) : 0;
        const double exp = (static_cast<double>(ca) + static_cast<double>(cb)) / 2.0;
        const double e = std::max(exp, 1.0);
        chi_a += (static_cast<double>(ca) - exp) * (static_cast<double>(ca) - exp) / e;
        chi_b += (static_cast<double>(cb) - exp) * (static_cast<double>(cb) - exp) / e;
    }
    for (const auto& [key, cb] : count_b) {
        if (count_a.count(key)) {
            continue;
        }
        const double exp = static_cast<double>(cb) / 2.0;
        const double e = std::max(exp, 1.0);
        chi_b += (static_cast<double>(cb) - exp) * (static_cast<double>(cb) - exp) / e;
    }
    const double df = static_cast<double>(std::max(count_a.size(), count_b.size()) - 1);
    const double crit = df + 3.0 * std::sqrt(2.0 * df);  // rough 99% chi upper bound
    if (chi_a > crit || chi_b > crit) {
        std::cerr << "chi-square pmf mismatch chi_a=" << chi_a << " chi_b=" << chi_b
                  << " crit~=" << crit << '\n';
        return false;
    }
    (void)alpha;
    return true;
}

bool check_discrete_pair(
    const int* tier_b, const int* tier_c, std::size_t n, const std::string& name) {
    const Stats sb = compute_stats(tier_b, n);
    const Stats sc = compute_stats(tier_c, n);
    if (!check_moments(sb, sc, n, 0.01)) {
        std::cerr << name << ": moment check failed\n";
        return false;
    }
    if (!check_discrete_chi(tier_b, tier_c, n, 0.01)) {
        std::cerr << name << ": chi-square check failed\n";
        return false;
    }
    return true;
}

bool check_continuous_pair(
    const double* tier_b, const double* tier_c, std::size_t n, const std::string& name) {
    const Stats sb = compute_stats(tier_b, n);
    const Stats sc = compute_stats(tier_c, n);
    if (!check_moments(sb, sc, n, 0.01)) {
        std::cerr << name << ": moment check failed\n";
        return false;
    }
    return true;
}

bool check_continuous_pair(
    const float* tier_b, const float* tier_c, std::size_t n, const std::string& name) {
    const Stats sb = compute_stats(tier_b, n);
    const Stats sc = compute_stats(tier_c, n);
    if (!check_moments(sb, sc, n, 0.01)) {
        std::cerr << name << ": moment check failed\n";
        return false;
    }
    return true;
}

template <typename TierB, typename TierC>
bool run_discrete_case(
    const std::string& name, TierB tier_b_fn, TierC tier_c_fn, std::uint64_t seed_b,
    std::uint64_t seed_c) {
    constexpr std::size_t n = 100'000;
    std::vector<int> b(n);
    std::vector<int> c(n);
    tier_b_fn(b.data(), n, seed_b);
    tier_c_fn(c.data(), n, seed_c);
    if (!check_discrete_pair(b.data(), c.data(), n, name)) {
        return false;
    }
    std::cout << name << ": Tier B vs Tier C ok\n";
    return true;
}

template <typename TierB, typename TierC>
bool run_continuous_case(
    const std::string& name, TierB tier_b_fn, TierC tier_c_fn, std::uint64_t seed_b,
    std::uint64_t seed_c) {
    constexpr std::size_t n = 100'000;
    std::vector<double> b(n);
    std::vector<double> c(n);
    tier_b_fn(b.data(), n, seed_b);
    tier_c_fn(c.data(), n, seed_c);
    if (!check_continuous_pair(b.data(), c.data(), n, name)) {
        return false;
    }
    std::cout << name << ": Tier B vs Tier C ok\n";
    return true;
}

template <typename TierB, typename TierC>
bool run_continuous_float_case(
    const std::string& name, TierB tier_b_fn, TierC tier_c_fn, std::uint64_t seed_b,
    std::uint64_t seed_c) {
    constexpr std::size_t n = 100'000;
    std::vector<float> b(n);
    std::vector<float> c(n);
    tier_b_fn(b.data(), n, seed_b);
    tier_c_fn(c.data(), n, seed_c);
    if (!check_continuous_pair(b.data(), c.data(), n, name)) {
        return false;
    }
    std::cout << name << ": Tier B vs Tier C ok\n";
    return true;
}

}  // namespace

int main() {
#if !defined(DISTRIBUTIONS_HAS_AVX2)
    std::cout << "skip: SIMD not compiled\n";
    return EXIT_SUCCESS;
#else
    if (!distributions::detail::simd::cpu_has_avx2()) {
        std::cout << "skip: CPU lacks AVX2\n";
        return EXIT_SUCCESS;
    }
    if (!distributions::detail::simd::tier_c_enabled()) {
        std::cerr << "tier_c_enabled() false on AVX2 CPU\n";
        return EXIT_FAILURE;
    }

    constexpr std::uint64_t seed_b = 42;
    constexpr std::uint64_t seed_c = 99;
    constexpr double probs[4] = {0.1, 0.2, 0.3, 0.4};

    if (!run_discrete_case(
            "bernoulli",
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::bernoulli_sample_batch(out, n, 0.35, seed);
            },
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::bernoulli_sample_batch(out, n, 0.35, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_discrete_case(
            "discrete-uniform",
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::discrete_uniform_sample_batch(out, n, 1, 7, seed);
            },
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::discrete_uniform_sample_batch(out, n, 1, 7, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_continuous_case(
            "exponential",
            [](double* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::exponential_sample_batch(out, n, 1.5, seed);
            },
            [](double* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::exponential_sample_batch(out, n, 1.5, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_continuous_case(
            "normal",
            [](double* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::normal_sample_batch(out, n, 1.0, 0.5, seed);
            },
            [](double* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::normal_sample_batch(out, n, 1.0, 0.5, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_continuous_float_case(
            "exponential-f32",
            [](float* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::exponential_sample_batch(out, n, 1.5, seed);
            },
            [](float* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::exponential_sample_batch(out, n, 1.5, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_continuous_float_case(
            "normal-f32",
            [](float* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::normal_sample_batch(out, n, 1.0, 0.5, seed);
            },
            [](float* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::normal_sample_batch(out, n, 1.0, 0.5, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_discrete_case(
            "binomial",
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::binomial_sample_batch(out, n, 20, 0.4, seed);
            },
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::binomial_sample_batch(out, n, 20, 0.4, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_discrete_case(
            "beta-binomial",
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::beta_binomial_sample_batch(out, n, 20, 2.0, 5.0, seed);
            },
            [](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::beta_binomial_sample_batch(
                    out, n, 20, 2.0, 5.0, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    if (!run_discrete_case(
            "poisson-binomial",
            [probs](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::fast::poisson_binomial_sample_batch(
                    out, n, probs, 4, seed);
            },
            [probs](int* out, std::size_t n, std::uint64_t seed) {
                distributions::detail::simd::poisson_binomial_sample_batch(
                    out, n, probs, 4, seed);
            },
            seed_b, seed_c)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
#endif
}
