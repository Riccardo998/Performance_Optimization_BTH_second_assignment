/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "filters.hpp"
#include "matrix.hpp"
#include "ppm.hpp"
#include <cmath>

/* 
 * Gaussian blur filter for images represented as Matrix objects
 * The blur is achieved using a Gaussian function, which mathematically defines the weight of neighboring pixels based on their distance from the
 * target pixel. The Gaussian function has a bell-shaped curve, meaning pixels closer to the center (the target pixel) have a higher weight, while
 * those further away have less influence
 */
namespace Filter
{

    namespace Gauss
    {
        /* This function calculates Gaussian weights for a given radius n and stores them in the weights_out array */
        void get_weights(int n, double *weights_out)
        {
            for (auto i{0}; i <= n; i++)
            {
                double x{static_cast<double>(i) * max_x / n};
                weights_out[i] = exp(-x * x * pi);
            }
        }
    }

    Matrix blur(Matrix m, const int radius)
    {
        /* A temporary matrix called scratch is created to hold intermediate results of the blur operation */
        Matrix scratch{PPM::max_dimension};
        auto dst{m}; // dst = destination

        /* Horizontal pass */
        /* The algorithm processes each pixel row by row, applying the Gaussian weights to the horizontal neighbors */
        for (auto x{0}; x < dst.get_x_size(); x++)
        {
            for (auto y{0}; y < dst.get_y_size(); y++)
            {
                /* Kernel creation */
                // TODO create a vector of radius lenght instead of maximum_length

                // TODO Leo told me to look at where get_weights is called
            
                double w[Gauss::max_radius]{}; // Note: max_radius is defined in filters.hpp
                Gauss::get_weights(radius, w);

                // unsigned char Matrix::r(unsigned x, unsigned y) const
                // {
                //     return R[y * x_size + x];
                // }

                /* Center pixel processing */
                /* r, g, b are accumulator variables, n is used for normalization */
                /* w[0] is the weight assigned to the center pixel(i.e. the pixel which is currently evaluated)*/
                auto r{w[0] * dst.r(x, y)}, 
                     g{w[0] * dst.g(x, y)}, 
                     b{w[0] * dst.b(x, y)}, 
                     n{w[0]};

                for (auto wi{1}; wi <= radius; wi++)
                {
                    auto wc{w[wi]};
                    auto x2{x - wi}; 
                    if (x2 >= 0) // left neighbour
                    {
                        r += wc * dst.r(x2, y); // accumulates weighted color values and weight sum
                        g += wc * dst.g(x2, y);
                        b += wc * dst.b(x2, y);
                        n += wc;
                    }
                    x2 = x + wi;
                    if (x2 < dst.get_x_size()) // right neighbour
                    {
                        r += wc * dst.r(x2, y);
                        g += wc * dst.g(x2, y);
                        b += wc * dst.b(x2, y);
                        n += wc;
                    }
                }
                /* Normalizes the accumulated colors by dividing by total weight */
                scratch.r(x, y) = r / n;
                scratch.g(x, y) = g / n;
                scratch.b(x, y) = b / n;
            }
        }
        /* Vertical pass */
        /* After the horizontal blur, it processes the image column by column, applying the Gaussian weights to the vertical neighbors. */
        for (auto x{0}; x < dst.get_x_size(); x++)
        {
            for (auto y{0}; y < dst.get_y_size(); y++)
            {
                double w[Gauss::max_radius]{};
                Gauss::get_weights(radius, w);

                auto r{w[0] * scratch.r(x, y)}, g{w[0] * scratch.g(x, y)}, b{w[0] * scratch.b(x, y)}, n{w[0]};

                for (auto wi{1}; wi <= radius; wi++)
                {
                    auto wc{w[wi]};
                    auto y2{y - wi};
                    if (y2 >= 0)
                    {
                        r += wc * scratch.r(x, y2);
                        g += wc * scratch.g(x, y2);
                        b += wc * scratch.b(x, y2);
                        n += wc;
                    }
                    y2 = y + wi;
                    if (y2 < dst.get_y_size())
                    {
                        r += wc * scratch.r(x, y2);
                        g += wc * scratch.g(x, y2);
                        b += wc * scratch.b(x, y2);
                        n += wc;
                    }
                }
                dst.r(x, y) = r / n;
                dst.g(x, y) = g / n;
                dst.b(x, y) = b / n;
            }
        }

        return dst;
    }

}
