/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "filters.hpp"
#include "matrix.hpp"
#include "ppm.hpp"
#include <cmath>
#include <immintrin.h>
#include <algorithm>

#include<cstdio>

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
//            // TODO - MinorOpt1: Loop unrolling
//            double c = max_x / n ;
//            double k1, x1, k2, x2;
//            char a = n % 2;
//            int last_index;
//            if (a == 0)
//            {
//
//                for (auto i{0}; i <= (n/2) -1  ; i+=2)
//                {
//                x1 = static_cast<double>(i) * c;
//                x2 = static_cast<double>(i+1) * c;
//
//                k1 = -x1 * x1 * pi;
//                k2 = -x2 * x2 * pi;
//
//                weights_out[i] = exp( k1 );
//                weights_out[i+1] = exp( k2 );
//
//                last_index = i+1;
//
//                }
//
//                x1 = static_cast<double>(last_index) * c;
//                k1 = -x1 * x1 * pi;
//                weights_out[last_index] = exp( k1 );
//            }
//            else
//            {
//                for (auto i{0}; i <= n/2 ; i+=2)
//                {
//                x1 = static_cast<double>(i) * c;
//                x2 = static_cast<double>(i+1) * c;
//
//                k1 = -x1 * x1 * pi;
//                k2 = -x2 * x2 * pi;
//
//                weights_out[i] = exp( k1 );
//                weights_out[i+1] = exp( k2 );
//                }
//            }
//
//
//        }
    }

Matrix blur(Matrix m, const int radius)
{
    Matrix scratch{PPM::max_dimension};
    auto dst{m};  // dst = destination

    // Pre-calculate weights
    double w[Gauss::max_radius]{};
    Gauss::get_weights(radius, w);

    // DEBUG
    // FILE* outputFile = fopen("output_image_opt.txt", "w");
    // if (!outputFile) {
    //     printf("Failed to open the file for writing.\n");
    //     return dst;
    // }

    /* Horizontal pass */
    // fprintf(outputFile, "Horizontal pass\n");
    for (auto x{0}; x < dst.get_x_size(); x++)
    {
        int y = 0;
        for (; y <= dst.get_y_size() - 4; y += 4)
        {
            __m256d r = _mm256_setzero_pd();
            __m256d g = _mm256_setzero_pd();
            __m256d b = _mm256_setzero_pd();
            __m256d n = _mm256_setzero_pd();

            __m256d w0 = _mm256_set1_pd(w[0]);
            __m256d r_center = _mm256_set_pd(dst.r(x, y + 3), dst.r(x, y + 2), dst.r(x, y + 1), dst.r(x, y));
            __m256d g_center = _mm256_set_pd(dst.g(x, y + 3), dst.g(x, y + 2), dst.g(x, y + 1), dst.g(x, y));
            __m256d b_center = _mm256_set_pd(dst.b(x, y + 3), dst.b(x, y + 2), dst.b(x, y + 1), dst.b(x, y));

            r = _mm256_add_pd(_mm256_mul_pd(w0, r_center), r);  // r = (w0 * r_center) + r
            g = _mm256_add_pd(_mm256_mul_pd(w0, g_center), g);  
            b = _mm256_add_pd(_mm256_mul_pd(w0, b_center), b);  
            n = _mm256_add_pd(n, w0);

            for (auto wi{1}; wi <= radius; wi++)
            {
                __m256d wc = _mm256_set1_pd(w[wi]);

                // Handle left neighbor
                int x2 = x - wi;
                if (x2 >= 0)
                {
                    __m256d r_left = _mm256_set_pd(dst.r(x2, y + 3), dst.r(x2, y + 2), dst.r(x2, y + 1), dst.r(x2, y));
                    __m256d g_left = _mm256_set_pd(dst.g(x2, y + 3), dst.g(x2, y + 2), dst.g(x2, y + 1), dst.g(x2, y));
                    __m256d b_left = _mm256_set_pd(dst.b(x2, y + 3), dst.b(x2, y + 2), dst.b(x2, y + 1), dst.b(x2, y));

                    r = _mm256_add_pd(_mm256_mul_pd(wc, r_left), r);  // r = (wc * r_left) + r
                    g = _mm256_add_pd(_mm256_mul_pd(wc, g_left), g);
                    b = _mm256_add_pd(_mm256_mul_pd(wc, b_left), b);
                    n = _mm256_add_pd(n, wc);
                }

                // Handle right neighbor
                x2 = x + wi;
                if (x2 < dst.get_x_size())
                {
                    __m256d r_right = _mm256_set_pd(dst.r(x2, y + 3), dst.r(x2, y + 2), dst.r(x2, y + 1), dst.r(x2, y));
                    __m256d g_right = _mm256_set_pd(dst.g(x2, y + 3), dst.g(x2, y + 2), dst.g(x2, y + 1), dst.g(x2, y));
                    __m256d b_right = _mm256_set_pd(dst.b(x2, y + 3), dst.b(x2, y + 2), dst.b(x2, y + 1), dst.b(x2, y));

                    r = _mm256_add_pd(_mm256_mul_pd(wc, r_right), r);  // r = (wc * r_right) + r
                    g = _mm256_add_pd(_mm256_mul_pd(wc, g_right), g);
                    b = _mm256_add_pd(_mm256_mul_pd(wc, b_right), b);               
                    n = _mm256_add_pd(n, wc);
                }
            }

            // Normalize the pixel values
            r = _mm256_div_pd(r, n);
            g = _mm256_div_pd(g, n);
            b = _mm256_div_pd(b, n);

            // Round to nearest integer
            // r = _mm256_round_pd(r, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
            // g = _mm256_round_pd(g, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
            // b = _mm256_round_pd(b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);

            // Store the result back into the matrix
            __m128i r_int = _mm256_cvtpd_epi32(r);
            __m128i g_int = _mm256_cvtpd_epi32(g);
            __m128i b_int = _mm256_cvtpd_epi32(b);

            int32_t r_vals[4], g_vals[4], b_vals[4];

            _mm_storeu_si128((__m128i*)r_vals, r_int);
            _mm_storeu_si128((__m128i*)g_vals, g_int);
            _mm_storeu_si128((__m128i*)b_vals, b_int);

            for (int i = 0; i < 4; ++i)
            {
                dst.r(x, y + i) = static_cast<unsigned char>(std::max(0, std::min(255, r_vals[i])));
                dst.g(x, y + i) = static_cast<unsigned char>(std::max(0, std::min(255, g_vals[i])));
                dst.b(x, y + i) = static_cast<unsigned char>(std::max(0, std::min(255, b_vals[i])));
                // Print the pixel index and the RGB values into the text file
                // fprintf(outputFile, "Pixel %d,%d: R = %d, G = %d, B = %d\n", x, y+i, dst.r(x, y + i), dst.g(x, y + i), dst.b(x, y + i));
            }

        }

        // Process remaining pixels (if y is not divisible by 4)
        for (; y < dst.get_y_size(); y++)
        {
            double r = w[0] * dst.r(x, y);
            double g = w[0] * dst.g(x, y);
            double b = w[0] * dst.b(x, y);
            double n = w[0];

            for (auto wi{1}; wi <= radius; wi++)
            {
                auto wc = w[wi];

                // Handle left neighbor
                int x2 = x - wi;
                if (x2 >= 0)
                {
                    r += wc * dst.r(x2, y);
                    g += wc * dst.g(x2, y);
                    b += wc * dst.b(x2, y);
                    n += wc;
                }

                // Handle right neighbor
                x2 = x + wi;
                if (x2 < dst.get_x_size())
                {
                    r += wc * dst.r(x2, y);
                    g += wc * dst.g(x2, y);
                    b += wc * dst.b(x2, y);
                    n += wc;
                }
            }

            // Normalize, round, and store
            dst.r(x, y) = static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(std::round(r / n)))));
            dst.g(x, y) = static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(std::round(g / n)))));
            dst.b(x, y) = static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(std::round(b / n)))));
            // fprintf(outputFile, "Pixel %d,%d: R = %d, G = %d, B = %d\n", x, y, dst.r(x, y), dst.g(x, y), dst.b(x, y));
        }
    }

    /* Vertical pass */
    // fprintf(outputFile, "Vertical pass\n");
    for (auto y{0}; y < dst.get_y_size(); y++)
    {
        int x = 0;
        for (; x <= dst.get_x_size() - 4; x += 4)
        {
            __m256d r = _mm256_setzero_pd();
            __m256d g = _mm256_setzero_pd();
            __m256d b = _mm256_setzero_pd();
            __m256d n = _mm256_setzero_pd();

            __m256d w0 = _mm256_set1_pd(w[0]);
            __m256d r_center = _mm256_set_pd(dst.r(x + 3, y), dst.r(x + 2, y), dst.r(x + 1, y), dst.r(x, y));
            __m256d g_center = _mm256_set_pd(dst.g(x + 3, y), dst.g(x + 2, y), dst.g(x + 1, y), dst.g(x, y));
            __m256d b_center = _mm256_set_pd(dst.b(x + 3, y), dst.b(x + 2, y), dst.b(x + 1, y), dst.b(x, y));

            r = _mm256_add_pd(_mm256_mul_pd(w0, r_center), r);  // r = (w0 * r_center) + r
            g = _mm256_add_pd(_mm256_mul_pd(w0, g_center), g);
            b = _mm256_add_pd(_mm256_mul_pd(w0, b_center), b);
            n = _mm256_add_pd(n, w0);

            for (auto wi{1}; wi <= radius; wi++)
            {
                __m256d wc = _mm256_set1_pd(w[wi]);

                // Handle above neighbor
                int y2 = y - wi;
                if (y2 >= 0)
                {
                    __m256d r_above = _mm256_set_pd(dst.r(x + 3, y2), dst.r(x + 2, y2), dst.r(x + 1, y2), dst.r(x, y2));
                    __m256d g_above = _mm256_set_pd(dst.g(x + 3, y2), dst.g(x + 2, y2), dst.g(x + 1, y2), dst.g(x, y2));
                    __m256d b_above = _mm256_set_pd(dst.b(x + 3, y2), dst.b(x + 2, y2), dst.b(x + 1, y2), dst.b(x, y2));

                    r = _mm256_add_pd(_mm256_mul_pd(wc, r_above), r);
                    g = _mm256_add_pd(_mm256_mul_pd(wc, g_above), g);
                    b = _mm256_add_pd(_mm256_mul_pd(wc, b_above), b);                   
                    n = _mm256_add_pd(n, wc);
                }

                // Handle below neighbor
                y2 = y + wi;
                if (y2 < dst.get_y_size())
                {
                    __m256d r_below = _mm256_set_pd(dst.r(x + 3, y2), dst.r(x + 2, y2), dst.r(x + 1, y2), dst.r(x, y2));
                    __m256d g_below = _mm256_set_pd(dst.g(x + 3, y2), dst.g(x + 2, y2), dst.g(x + 1, y2), dst.g(x, y2));
                    __m256d b_below = _mm256_set_pd(dst.b(x + 3, y2), dst.b(x + 2, y2), dst.b(x + 1, y2), dst.b(x, y2));

                    r = _mm256_add_pd(_mm256_mul_pd(wc, r_below), r);
                    g = _mm256_add_pd(_mm256_mul_pd(wc, g_below), g);
                    b = _mm256_add_pd(_mm256_mul_pd(wc, b_below), b);
                    n = _mm256_add_pd(n, wc);
                }
            }

            // Normalize and round the pixel values
            r = _mm256_div_pd(r, n);
            g = _mm256_div_pd(g, n);
            b = _mm256_div_pd(b, n);

            // r = _mm256_round_pd(r, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
            // g = _mm256_round_pd(g, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
            // b = _mm256_round_pd(b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);

            // Store the result back into the matrix
            __m128i r_int = _mm256_cvtpd_epi32(r);
            __m128i g_int = _mm256_cvtpd_epi32(g);
            __m128i b_int = _mm256_cvtpd_epi32(b);

            int32_t r_vals[4], g_vals[4], b_vals[4];
            _mm_storeu_si128((__m128i*)r_vals, r_int);
            _mm_storeu_si128((__m128i*)g_vals, g_int);
            _mm_storeu_si128((__m128i*)b_vals, b_int);

            for (int i = 0; i < 4; ++i)
            {
                dst.r(x + i, y) = static_cast<unsigned char>(std::max(0, std::min(255, r_vals[i])));
                dst.g(x + i, y) = static_cast<unsigned char>(std::max(0, std::min(255, g_vals[i])));
                dst.b(x + i, y) = static_cast<unsigned char>(std::max(0, std::min(255, b_vals[i])));
                // fprintf(outputFile, "Pixel %d,%d: R = %d, G = %d, B = %d\n", x, y+i, dst.r(x, y + i), dst.g(x, y + i), dst.b(x, y + i));
            }
        }

        // Process remaining pixels (if x is not divisible by 4)
        for (; x < dst.get_x_size(); x++)
        {
            double r = w[0] * dst.r(x, y);
            double g = w[0] * dst.g(x, y);
            double b = w[0] * dst.b(x, y);
            double n = w[0];

            for (auto wi{1}; wi <= radius; wi++)
            {
                auto wc = w[wi];

                // Handle above neighbor
                int y2 = y - wi;
                if (y2 >= 0)
                {
                    r += wc * dst.r(x, y2);
                    g += wc * dst.g(x, y2);
                    b += wc * dst.b(x, y2);
                    n += wc;
                }

                // Handle below neighbor
                y2 = y + wi;
                if (y2 < dst.get_y_size())
                {
                    r += wc * dst.r(x, y2);
                    g += wc * dst.g(x, y2);
                    b += wc * dst.b(x, y2);
                    n += wc;
                }
            }

            // Normalize, round, and store
            dst.r(x, y) = static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(std::round(r / n)))));
            dst.g(x, y) = static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(std::round(g / n)))));
            dst.b(x, y) = static_cast<unsigned char>(std::max(0, std::min(255, static_cast<int>(std::round(b / n)))));
            // fprintf(outputFile, "Pixel %d,%d: R = %d, G = %d, B = %d\n", x, y, dst.r(x, y), dst.g(x, y), dst.b(x, y));
        }
    }

    // fclose(outputFile);
    return dst;
}



}