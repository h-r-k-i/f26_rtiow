#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double random_double() {
    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

struct Cell {
    int w0, w1;
    int h0, h1;
};

#include "color.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#endif // RTWEEKEND_HPP