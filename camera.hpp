#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <atomic>
#include <thread>

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

        void render(const hittable& world, std::vector<color>& buffer, std::vector<Cell>& cells) {
            initialize();
            std::atomic<size_t> next_cell(0);
            std::atomic<size_t> complete(0);
            const int VirtualCores = std::thread::hardware_concurrency();

            std::vector<std::thread> threads;

            for (int i = 0; i < VirtualCores; i++) {
                threads.emplace_back([&]() {
                    while (true) {
                        size_t cell_idx = next_cell.fetch_add(1);
                        if (cell_idx > cells.size()) break;

                        render_thread(buffer, cells[cell_idx], world);
                        size_t done = complete.fetch_add(1);
                        std::clog << "\rCells completed: " << done << '/' << cells.size() << "           " << std::flush;
                    }
                });
            }

            for (auto& thread : threads) {
                
                thread.join();
            }

            std::clog << "\rRendering done.                                                                    \n";
        }

        int getImageHeight() {
            if (!initialized) initialize();
            return imageHeight;
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
        bool initialized = false;

        void initialize() {
            initialized = true;
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
            auto ray_time = random_double();

            return ray(ray_origin, ray_direction, ray_time);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            color accumulated_color(1, 1, 1);
            ray current_ray = r;
            
            for (int i = 0; i < depth; i++) {
                hit_record rec;
                
                if (world.hit(current_ray, interval(0.001, infinity), rec)) {
                    ray scattered;
                    color attenuation;
                    
                    if (rec.mat->scatter(current_ray, rec, attenuation, scattered)) {
                        accumulated_color = accumulated_color * attenuation;
                        current_ray = scattered;
                    } else {
                        return color(0, 0, 0);
                    }
                } else {
                    vec3 unitDirection = unit_vector(current_ray.direction());
                    auto a = 0.5 * (unitDirection.y() + 1.0);
                    color sky = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
                    return accumulated_color * sky;
                }
            }
            return color(0, 0, 0);
        }

        void render_thread(std::vector<color>& buffer, Cell cell, const hittable& world) {
            for (int j = cell.h0; j < cell.h1; j++) {
                // std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = cell.w0; i < cell.w1; i++) {
                    color px(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        px += ray_color(r, max_depth, world);
                    }
                    buffer[j * imageWidth + i] = pixel_samples_scale * px;
                }
            }

            // std::clog << "\rRendering done.                 \n";
        }

};

#endif // CAMERA_HPP