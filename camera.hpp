#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"
#include "material.hpp"

class camera {
    public:
        double aspectratio = 1.l;
        int    imageWidth = 400;
        int samples_per_pixel = 10;
        int max_depth = 10;
        double vfov = 90;
        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0,0, -1);
        vec3 vup = vec3(0, 1, 0);
        double defocus_angle = 0;
        double focus_dist = 10;

        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++) {
                    color pixel(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel);
                }
            }

            std::clog << "\rDone.                 \n";
        }
    
    private:
        int imageHeight;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixelDelta_u;
        vec3 pixelDelta_v;
        vec3 u, v, w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void initialize() {
            imageHeight = int(imageWidth / aspectratio);
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta / 2);
            auto viewportHeight = 2.l * h * focus_dist;
            auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            vec3 viewport_u = viewportWidth * u;
            vec3 viewport_v = viewportHeight * -v;

            pixelDelta_u = viewport_u / imageWidth;
            pixelDelta_v = viewport_v / imageHeight;

            auto viewportUpperLeft =
                center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewportUpperLeft + 0.5 * (pixelDelta_u + pixelDelta_v);

            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = defocus_radius * u;
            defocus_disk_v = defocus_radius * v;
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                                + ((i + offset.x()) * pixelDelta_u)
                                + ((j + offset.y()) * pixelDelta_v);
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0) return color(0, 0, 0);
            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth - 1, world);
                return color(0, 0, 0);
            }
            
            vec3 unitDirection = unit_vector(r.direction());
            auto a = 0.5 * (unitDirection.y() + 1.l);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }

};

#endif // CAMERA_HPP