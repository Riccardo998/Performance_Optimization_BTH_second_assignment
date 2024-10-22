/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "filters.hpp"
#include "matrix.hpp"
#include "ppm.hpp"
#include <cmath>

namespace Filter
{

    namespace Gauss
    {
        void get_weights(int n, double *weights_out)
        {
            for (auto i{0}; i <= n; i++)
            {
                double x{static_cast<double>(i) * max_x / n};
                weights_out[i] = exp(-x * x * pi);
            }
        }
    }

    Matrix blur(Matrix& m, const int radius)
    {
        Matrix scratch{PPM::max_dimension};
        auto dst{m}; // dst = destination
    
        auto x_size = dst.get_x_size();
        auto y_size = dst.get_y_size();
    
        /* Horizontal pass */
        for (auto x{0}; x < x_size; x += 4)
        {
            for (auto y{0}; y < y_size; y++)
            {
                double w[Gauss::max_radius]{};
                Gauss::get_weights(radius, w);
    
                // Process 4 pixels in one iteration if available
                for (int offset = 0; offset < 4 && (x + offset) < x_size; ++offset) {
                    auto r = w[0] * dst.r(x + offset, y); 
                    auto g = w[0] * dst.g(x + offset, y); 
                    auto b = w[0] * dst.b(x + offset, y); 
                    auto n = w[0];
    
                    for (auto wi{1}; wi <= radius; wi++)
                    {
                        auto wc = w[wi];
                        auto x2 = x + offset - wi; 
                        if (x2 >= 0) // left neighbour
                        {
                            r += wc * dst.r(x2, y);
                            g += wc * dst.g(x2, y);
                            b += wc * dst.b(x2, y);
                            n += wc;
                        }
                        x2 = x + offset + wi;
                        if (x2 < x_size) // right neighbour
                        {
                            r += wc * dst.r(x2, y);
                            g += wc * dst.g(x2, y);
                            b += wc * dst.b(x2, y);
                            n += wc;
                        }
                    }
                    scratch.r(x + offset, y) = r / n;
                    scratch.g(x + offset, y) = g / n;
                    scratch.b(x + offset, y) = b / n;
                }
            }
        }
    
        /* Vertical pass */
        for (auto x{0}; x < x_size; x += 4)
        {
            for (auto y{0}; y < y_size; y++)
            {
                double w[Gauss::max_radius]{};
                Gauss::get_weights(radius, w);
    
                // Process 4 pixels in one iteration if available
                for (int offset = 0; offset < 4 && (x + offset) < x_size; ++offset) {
                    auto r = w[0] * scratch.r(x + offset, y); 
                    auto g = w[0] * scratch.g(x + offset, y); 
                    auto b = w[0] * scratch.b(x + offset, y); 
                    auto n = w[0];
    
                    for (auto wi{1}; wi <= radius; wi++)
                    {
                        auto wc = w[wi];
                        auto y2 = y - wi;
                        if (y2 >= 0)
                        {
                            r += wc * scratch.r(x + offset, y2);
                            g += wc * scratch.g(x + offset, y2);
                            b += wc * scratch.b(x + offset, y2);
                            n += wc;
                        }
                        y2 = y + wi;
                        if (y2 < y_size)
                        {
                            r += wc * scratch.r(x + offset, y2);
                            g += wc * scratch.g(x + offset, y2);
                            b += wc * scratch.b(x + offset, y2);
                            n += wc;
                        }
                    }
                    dst.r(x + offset, y) = r / n;
                    dst.g(x + offset, y) = g / n;
                    dst.b(x + offset, y) = b / n;
                }
            }
        }
    
        return dst;
    }
}
