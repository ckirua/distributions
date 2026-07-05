#include "dispatch.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: sample_dump <dist> <seed> <n> <out.bin>\n";
        return 1;
    }
    const std::string dist = argv[1];
    const std::uint64_t seed = static_cast<std::uint64_t>(std::strtoull(argv[2], nullptr, 10));
    const std::size_t n = static_cast<std::size_t>(std::strtoull(argv[3], nullptr, 10));
    const std::string path = argv[4];

    bool discrete = true;
    for (const auto& d : bench::all_dists()) {
        if (d.id == dist) {
            discrete = d.discrete;
            break;
        }
    }

    std::vector<int> out_i;
    std::vector<double> out_d;
    bench::sample_cpp(dist, seed, n, out_i, out_d);
    bench::write_samples_binary(path, discrete, out_i, out_d);
    return 0;
}
