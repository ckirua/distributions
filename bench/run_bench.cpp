#include "dispatch.hpp"

#include "harness.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Options {
    std::string dist{"bernoulli"};
    std::size_t n{1'000'000};
    std::uint64_t seed{42};
    bool csv{false};
};

Options parse_args(int argc, char** argv) {
    Options opt;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--csv") {
            opt.csv = true;
        }
    }
    if (argc >= 2 && argv[1][0] != '-') {
        opt.dist = argv[1];
    }
    if (argc >= 3 && argv[2][0] != '-') {
        opt.n = static_cast<std::size_t>(std::strtoull(argv[2], nullptr, 10));
    }
    if (argc >= 4 && argv[3][0] != '-') {
        opt.seed = static_cast<std::uint64_t>(std::strtoull(argv[3], nullptr, 10));
    }
    return opt;
}

bool is_discrete(const std::string& dist) {
    for (const auto& d : bench::all_dists()) {
        if (d.id == dist) {
            return d.discrete;
        }
    }
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc >= 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cerr << "Usage: run_bench <dist> [n] [seed] [--csv]\n";
        return 0;
    }

    const Options opt = parse_args(argc, argv);
    const bool discrete = is_discrete(opt.dist);
    std::vector<int> out_i;
    std::vector<double> out_d;
    if (discrete) {
        out_i.resize(opt.n);
    } else {
        out_d.resize(opt.n);
    }

    const auto run_kernel = [&]() {
        bench::sample_cpp(opt.dist, opt.seed, opt.n, out_i, out_d);
    };

    // Baseline: touch same output buffer (fair vs kernel memory traffic).
    const auto baseline = bench::measure_cycles([&]() {
        if (discrete) {
            for (std::size_t i = 0; i < opt.n; ++i) {
                volatile int sink = out_i[i];
                (void)sink;
            }
        } else {
            for (std::size_t i = 0; i < opt.n; ++i) {
                volatile double sink = out_d[i];
                (void)sink;
            }
        }
    });
    const auto timed = bench::measure_cycles(run_kernel);
    const double net_median = std::max(0.0, timed.median_cycles - baseline.median_cycles);
    const double net_p99 = std::max(0.0, timed.p99_cycles - baseline.p99_cycles);

    if (opt.csv) {
        std::cout << opt.dist << ",cpp," << opt.n << ',' << opt.seed << ',' << net_median << ','
                  << net_p99 << ',' << timed.stddev_cycles << ','
                  << (net_median / static_cast<double>(opt.n)) << '\n';
    } else {
        std::cout << "dist=" << opt.dist << " backend=cpp n=" << opt.n << " seed=" << opt.seed << '\n'
                  << "median=" << net_median << " p99=" << net_p99
                  << " stddev=" << timed.stddev_cycles << " per_sample="
                  << (net_median / static_cast<double>(opt.n)) << '\n';
    }
    return 0;
}
