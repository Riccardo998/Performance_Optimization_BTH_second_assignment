/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "vector.hpp"
#include <iostream>
#include <cmath>
#include <vector>

Vector::Vector()
    : size{0}, data{nullptr}
{
}

Vector::~Vector()
{
    if (data)
    {
        delete[] data;
    }

    size = 0;
}

Vector::Vector(unsigned size)
    : size{size}, data{new double[size]}
{
}

Vector::Vector(unsigned size, double *data)
    : size{size}, data{data}
{
}

Vector::Vector(const Vector &other)
    : Vector{other.size}
{
    for (auto i{0}; i < size; i++)
    {
        data[i] = other.data[i];
    }
}

unsigned Vector::get_size() const
{
    return size;
}

double *Vector::get_data()
{
    return data;
}

double Vector::operator[](unsigned i) const
{
    return data[i];
}

double &Vector::operator[](unsigned i)
{
    return data[i];
}

double Vector::mean() const
{
    double sum{0};

    for (auto i{0}; i < size; i++)
    {
        sum += data[i];
    }

    return sum / static_cast<double>(size);
}

/* The square root of the sum of the squares of the components gives the magnitude (or length) of the vector */
/* we are returning the square root of the dot product this x this */
double Vector::magnitude() const
{
    auto dot_prod{dot(*this)};
    return std::sqrt(dot_prod);
}


Vector Vector::operator/(double div)
{
    auto result{*this}; 

    unsigned i = 0;

    // Loop unrolling 
    for (; i + 4 <= size; i += 4) {
        result[i] /= div;
        result[i + 1] /= div;
        result[i + 2] /= div;
        result[i + 3] /= div;
    }
    // remaining elements
    for (; i < size; ++i) {
        result[i] /= div;
    }

    return result; // Return the new vector
}



Vector Vector::operator-(double sub)
{
    Vector result(size);
    unsigned i = 0;

    // Loop unrolling for subtraction
    for (; i + 4 <= size; i += 4) {
        result[i] = data[i] - sub;
        result[i + 1] = data[i + 1] - sub;
        result[i + 2] = data[i + 2] - sub;
        result[i + 3] = data[i + 3] - sub;
    }
    // Handle remaining elements
    for (; i < size; ++i) {
        result[i] = data[i] - sub;
    }

    return result; // Return the new vector with the subtracted values
}


double Vector::dot(Vector rhs) const
{
    double result{0};
    unsigned i = 0;

    // Loop unrolling for dot product
    for (; i + 4 <= size; i += 4) {
        result += data[i] * rhs[i];
        result += data[i + 1] * rhs[i + 1];
        result += data[i + 2] * rhs[i + 2];
        result += data[i + 3] * rhs[i + 3];
    }
    // Handle remaining elements
    for (; i < size; ++i) {
        result += data[i] * rhs[i];
    }

    return result; // Return the computed dot product
}

