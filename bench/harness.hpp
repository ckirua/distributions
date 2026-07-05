#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <thread>
#include <vector>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#include <x86intrin.h>
#endif

namespace bench {

struct TimingStats {
    double median_cycles{0.0};
    double p99_cycles{0.0};
    double stddev_cycles{0.0};
    std::size_t iterations{0};
};

inline std::uint64_t rdtsc() {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    return __rdtsc();
#else
    return static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
#endif
}

inline double calibrate_cycles_per_ns() {
    constexpr int kSamples = 5;
    double total = 0.0;
    for (int s = 0; s < kSamples; ++s) {
        const auto t0 = std::chrono::steady_clock::now();
        const std::uint64_t c0 = rdtsc();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        const auto t1 = std::chrono::steady_clock::now();
        const std::uint64_t c1 = rdtsc();
        const double ns = static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
        total += static_cast<double>(c1 - c0) / ns;
    }
    return total / static_cast<double>(kSamples);
}

template <typename Fn>
TimingStats measure_cycles(Fn&& fn, std::size_t warmup = 3, std::size_t samples = 30) {
    for (std::size_t i = 0; i < warmup; ++i) {
        fn();
    }

    std::vector<double> cycles;
    cycles.reserve(samples);
    for (std::size_t i = 0; i < samples; ++i) {
        const std::uint64_t t0 = rdtsc();
        fn();
        const std::uint64_t t1 = rdtsc();
        cycles.push_back(static_cast<double>(t1 - t0));
    }

    std::sort(cycles.begin(), cycles.end());
    const double median = cycles[cycles.size() / 2];
    const std::size_t p99_idx =
        std::min(cycles.size() - 1, static_cast<std::size_t>(cycles.size() * 0.99));
    const double p99 = cycles[p99_idx];

    double mean = 0.0;
    for (double c : cycles) {
        mean += c;
    }
    mean /= static_cast<double>(cycles.size());
    double var = 0.0;
    for (double c : cycles) {
        const double d = c - mean;
        var += d * d;
    }
    var /= static_cast<double>(cycles.size());

    return TimingStats{median, p99, std::sqrt(var), samples};
}

}  // namespace bench
