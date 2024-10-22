/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis.hpp"
#include "dataset.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char const* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [dataset] [outfile] [num_threads]" << std::endl;
        std::exit(1);
    }

    int num_threads = std::atoi(argv[3]);
    if (num_threads != 2 && num_threads != 4 && num_threads != 8 && num_threads != 16 && num_threads != 32) {
        std::cerr << "Error: Number of threads must be 2, 4, 8, 16, or 32." << std::endl;
        std::exit(1);
    }

    auto datasets { Dataset::read(argv[1]) };
    auto corrs { Analysis::correlation_coefficients(datasets, num_threads) };
    Dataset::write(corrs, argv[2]);

    return 0;
}
