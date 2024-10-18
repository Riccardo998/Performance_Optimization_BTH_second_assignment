/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "dataset.hpp"
#include "vector.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <limits>

namespace Dataset
{
    /* Read dataset from filename, store each floating point in a dimension x dimension matrix */
    std::vector<Vector> read(std::string filename)
    {
        unsigned dimension{}; // store how many numbers each vector contains
        std::vector<Vector> result{}; // dynamic list of Vectors --> vector of Vectors --> a dimension x dimenstion matrix
        std::ifstream f{};

        f.open(filename);

        if (!f)
        {
            std::cerr << "Failed to read dataset(s) from file " << filename << std::endl;
            return result;
        }

        f >> dimension;
        std::string line{};

        std::getline(f, line); // ignore first newline

        while (std::getline(f, line))
        {
            std::stringstream ss{line}; //create a stream from the line (makes it easier to parse numbers)
            Vector new_vec{dimension};  // empty Vector of dimension elements

            /* std::copy_n(InputIterator first, Size n, OutputIterator result) */
            std::copy_n(std::istream_iterator<double>{ss}, // creates an iterator that reads doubles from the stringstream
                        dimension,
                        new_vec.get_data());
            result.push_back(new_vec);
        }

        return result;
    }

    /* write the vector data to filename */
    void write(std::vector<double> data, std::string filename)
    {
        std::ofstream f{};

        f.open(filename);

        if (!f)
        {
            std::cerr << "Failed to write data to file " << filename << std::endl;
            return;
        }

        for (auto i{0}; i < data.size(); i++)
        {
            f << std::setprecision(std::numeric_limits<double>::digits10 + 1) << data[i] << std::endl;
        }
    }

};
