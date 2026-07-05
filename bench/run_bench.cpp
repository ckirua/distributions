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
    bench::Backend backend{bench::Backend::Cpp};
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
        } else if (arg == "cpp") {
            opt.backend = bench::Backend::Cpp;
        } else if (arg == "ispc") {
            opt.backend = bench::Backend::Ispc;
        } else if (opt.dist == "bernoulli" && opt.n == 1'000'000 && opt.seed == 42 &&
                   !arg.empty() && arg[0] != '-') {
            // first positional
            if (i == 1) {
                opt.dist = arg;
            } else if (i == 2 || (i == 3 && std::string(argv[2]) != "cpp" &&
                                  std::string(argv[2]) != "ispc")) {
                // handled below
            }
        }
    }
    if (argc >= 2 && argv[1][0] != '-') {
        opt.dist = argv[1];
    }
    if (argc >= 3) {
        const std::string b = argv[2];
        if (b == "cpp") {
            opt.backend = bench::Backend::Cpp;
        } else if (b == "ispc") {
            opt.backend = bench::Backend::Ispc;
        }
    }
    if (argc >= 4) {
        opt.n = static_cast<std::size_t>(std::strtoull(argv[3], nullptr, 10));
    }
    if (argc >= 5) {
        opt.seed = static_cast<std::uint64_t>(std::strtoull(argv[4], nullptr, 10));
    }
    return opt;
}

bool is_discrete(const std::string& dist) {
    for (const auto& d : bench::phase1_dists()) {
        if (d.id == dist) {
            return d.discrete;
        }
    }
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc >= 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cerr << "Usage: run_bench <dist> [cpp|ispc] [n] [seed] [--csv]\n";
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
        if (opt.backend == bench::Backend::Ispc) {
            bench::sample_ispc(opt.dist, opt.seed, opt.n, out_i, out_d);
        } else {
            bench::sample_cpp(opt.dist, opt.backend, opt.seed, opt.n, out_i, out_d);
        }
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
        std::cout << opt.dist << ',' << (opt.backend == bench::Backend::Cpp ? "cpp" : "ispc") << ','
                  << opt.n << ',' << opt.seed << ',' << net_median << ',' << net_p99 << ','
                  << timed.stddev_cycles << ',' << (net_median / static_cast<double>(opt.n))
                  << '\n';
    } else {
        std::cout << "dist=" << opt.dist
                  << " backend=" << (opt.backend == bench::Backend::Cpp ? "cpp" : "ispc")
                  << " n=" << opt.n << " seed=" << opt.seed << '\n'
                  << "median=" << net_median << " p99=" << net_p99
                  << " stddev=" << timed.stddev_cycles << " per_sample="
                  << (net_median / static_cast<double>(opt.n)) << '\n';
    }
    return 0;
}
