#include "dispatch.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
    for (const auto& spec : bench::all_dists()) {
        try {
            std::vector<int> out_i;
            std::vector<double> out_d;
            bench::sample_cpp(spec.id, bench::Backend::Cpp, 42, 64, out_i, out_d);
            const std::size_t n = spec.discrete ? out_i.size() : out_d.size();
            if (n != 64) {
                std::cerr << spec.id << ": expected 64 samples, got " << n << "\n";
                return EXIT_FAILURE;
            }
        } catch (const std::exception& e) {
            std::cerr << spec.id << ": " << e.what() << "\n";
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
