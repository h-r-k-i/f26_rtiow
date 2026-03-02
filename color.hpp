#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) return std::sqrt(linear_component);
    return 0;
}

void write_color(std::ostream& out, const color& pixel) {
    auto r = pixel.x();
    auto g = pixel.y();
    auto b = pixel.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.l, 0.999);
    int ri = int(256 * intensity.clamp(r));
    int gi = int(256 * intensity.clamp(g));
    int bi = int(256 * intensity.clamp(b));

    unsigned char rb = (unsigned char)ri;
    unsigned char gb = (unsigned char)gi;
    unsigned char bb = (unsigned char)bi;

    out << rb << gb << bb;
}

#endif // COLOR_HPP