#include "dispatch.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: sample_dump <dist> <cpp|ispc> <seed> <n> <out.bin>\n";
        return 1;
    }
    const std::string dist = argv[1];
    const bench::Backend backend =
        std::string(argv[2]) == "ispc" ? bench::Backend::Ispc : bench::Backend::Cpp;
    const std::uint64_t seed = static_cast<std::uint64_t>(std::strtoull(argv[3], nullptr, 10));
    const std::size_t n = static_cast<std::size_t>(std::strtoull(argv[4], nullptr, 10));
    const std::string path = argv[5];

    bool discrete = true;
    for (const auto& d : bench::phase1_dists()) {
        if (d.id == dist) {
            discrete = d.discrete;
            break;
        }
    }

    std::vector<int> out_i;
    std::vector<double> out_d;
    if (backend == bench::Backend::Ispc) {
        bench::sample_ispc(dist, seed, n, out_i, out_d);
    } else {
        bench::sample_cpp(dist, backend, seed, n, out_i, out_d);
    }
    bench::write_samples_binary(path, discrete, out_i, out_d);
    return 0;
}
