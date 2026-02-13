#include <fstream>

#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittablelist.hpp"
#include "material.hpp"
#include "sphere.hpp"

const int tileSize = 32;

int main() {

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -22; a < 22; a++) {
        for (int b = -22; b < 22; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0.l, 0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspectratio = 16.l / 9.l;
    cam.imageWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 5;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    auto file = "image2.ppm";
    std::ofstream File(file);

    File << "P3\n" << cam.imageWidth << ' ' << (int)(cam.imageWidth / cam.aspectratio) << "\n255\n";

    std::vector<color> framebuffer(cam.imageWidth * (cam.imageWidth / cam.aspectratio));
    int imageHeight = (cam.imageWidth / cam.aspectratio);
    int cBx = (int)(std::ceil((double)cam.imageWidth / tileSize));
    int cBy = (int)(std::ceil(((double)imageHeight / tileSize)));
    std::vector<Cell> cellBuffer;


    for (int y = 0; y < cBy; y++) {
        for (int x = 0; x < cBx; x++) {
            cellBuffer.push_back(Cell{
                x * tileSize,
                std::min((x + 1) * tileSize, cam.imageWidth),
                y * tileSize,
                std::min((y + 1) * tileSize, imageHeight)
            });
        }
    }


    cam.render(world, framebuffer, cellBuffer);

    auto pixelCount = framebuffer.size();

    for (auto pixel : framebuffer) {
        std::clog << "\rPixels remaining: " << (pixelCount--) << ' ' <<  std::flush;
        write_color(File, pixel);
    }
    std::clog << "\rWriting to " << file << " done.                   \n";

    File.close();
}

