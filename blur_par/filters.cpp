/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "filters.hpp"
#include "matrix.hpp"
#include "ppm.hpp"
#include <cmath>

#include <pthread.h>
#include <vector>
#include <cmath>

struct ThreadData {
    const Matrix* src; // source matrix
    Matrix* dst;       // destination matrix
    Matrix* scratch;   // intermediate matrix
    int start_x;       // start x coord 
    int end_x;         // end x coord
    int y_size;        // image height
    int radius;        // blurring radius
    int x_size;        // image witdh
};

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

void* horizontal_blur(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    const Matrix& src = *data->src;
    Matrix& scratch = *data->scratch;
    int y_size = data->y_size;
    int radius = data->radius;
    int x_size = data->x_size;  // Use full x_size for correct boundary checks

    double w[Gauss::max_radius]{};
    Gauss::get_weights(radius, w);

    // apply horizontal blurring for each pixel in the thread chunk
    for (auto x = data->start_x; x < data->end_x; ++x) {
        for (auto y = 0; y < y_size; ++y) {
            
            auto r = w[0] * src.r(x, y);
            auto g = w[0] * src.g(x, y);
            auto b = w[0] * src.b(x, y);
            auto n = w[0];

            for (auto wi = 1; wi <= radius; wi++) {
                auto wc = w[wi];
                auto x_left = x - wi;
                auto x_right = x + wi;
                if (x_left >= 0) {
                    r += wc * src.r(x_left, y);
                    g += wc * src.g(x_left, y);
                    b += wc * src.b(x_left, y);
                    n += wc;
                }
                if (x_right < x_size) {
                    r += wc * src.r(x_right, y);
                    g += wc * src.g(x_right, y);
                    b += wc * src.b(x_right, y);
                    n += wc;
                }
            }
            scratch.r(x, y) = r / n;
            scratch.g(x, y) = g / n;
            scratch.b(x, y) = b / n;
        }
    }
    return nullptr;
}

void* vertical_blur(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    const Matrix& scratch = *data->scratch;
    Matrix& dst = *data->dst;
    int y_size = data->y_size;
    int radius = data->radius;

    double w[Gauss::max_radius]{};
    Gauss::get_weights(radius, w);

    // apply vertical blurring for each pixel in the thread chunk
    for (auto x = data->start_x; x < data->end_x; ++x) {
        for (auto y = 0; y < y_size; ++y) {
            
            auto r = w[0] * scratch.r(x, y);
            auto g = w[0] * scratch.g(x, y);
            auto b = w[0] * scratch.b(x, y);
            auto n = w[0];

            for (auto wi = 1; wi <= radius; wi++) {
                auto wc = w[wi];
                auto y_up = y - wi;
                auto y_down = y + wi;
                if (y_up >= 0) {
                    r += wc * scratch.r(x, y_up);
                    g += wc * scratch.g(x, y_up);
                    b += wc * scratch.b(x, y_up);
                    n += wc;
                }
                if (y_down < y_size) {
                    r += wc * scratch.r(x, y_down);
                    g += wc * scratch.g(x, y_down);
                    b += wc * scratch.b(x, y_down);
                    n += wc;
                }
            }
            dst.r(x, y) = r / n;
            dst.g(x, y) = g / n;
            dst.b(x, y) = b / n;
        }
    }
    return nullptr;
}

Matrix blur(Matrix& m, const int radius, int num_threads) {
    // if (num_threads != 2 && num_threads != 4 && num_threads != 8 && num_threads != 16 && num_threads != 32) {
    //     throw std::invalid_argument("Number of threads must be 2, 4, 8, 16, or 32");
    // }

    Matrix scratch{PPM::max_dimension};
    Matrix dst{m}; // dst = destination
    int x_size = dst.get_x_size();
    int y_size = dst.get_y_size();

    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    int chunk_size = (x_size + num_threads - 1) / num_threads;

    // horizontal pass
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i] = {
            &m, &dst, &scratch,
            i * chunk_size,
            std::min((i + 1) * chunk_size, x_size),
            y_size, radius, x_size
        };
        pthread_create(&threads[i], nullptr, horizontal_blur, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // vertical pass
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i] = {
            &m, &dst, &scratch,
            i * chunk_size,
            std::min((i + 1) * chunk_size, x_size),
            y_size, radius, x_size
        };
        pthread_create(&threads[i], nullptr, vertical_blur, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return dst;
}
}
