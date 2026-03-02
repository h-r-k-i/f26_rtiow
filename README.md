# February 2026 Project: Ray Tracing in One Weekend (Books 1 + 2)
Peter Shirley's *Ray Tracing in One Weekend* and *Ray Tracing: The Next Week*, implemented (presumably poorly) in C++. I state the aforementioned parenthetical because of apparent issues with rendering time, which are likely my fault.

This project implements with some success *Ray Tracing in One Weekend* with its basic CPU-heavy raytracing pipeline, anti-aliasing, various materials, and a camera; and *Ray Tracing: The Next Week* with motion blue, bounding volume hierarchies, teture mapping, geometric objects that are not spheres, basic transformations, and more materials.

Alongside these, this project also implements a cell-based multiprocessing system to allow the program to use all the power of the CPU to produce an image (and heat an entire building in the process), and writes to a framebuffer directly instead of relying on the books' method of printing to standard output and redirecting that output to a PNM file in the console.

## Getting Started

If somehow you want to run this project yourself instead of just getting this project from the far-more-competent [Ray Tracing in One Weekend GitHub](https://github.com/RayTracing/raytracing.github.io), this is possible (in a sense).

### Prerequisites
* C++ compiler. If using GCC, you may use the Makefile I've decided to leave in the project because I can't be bothered to learn how to properly use gitignore. If not using GCC, don't use the Makefile C++11 support has been verified.
* CMake, if you want to use that.

### Building
```bash
# Clone repo
git clone https://github.com/h-r-k-i/f26_rtiow.git
cd f26_rtiow

# Build with command line (replace g++ with preferred compiler)
g++ -std=c++11 -O3 main.cpp -o raytracer

# Or use CMake (according to Claude; I don't use CMake)
mkdir build && cd build
cmake ..
make

# Or use the Makefile in the directory if you're brave enough
make
```
### Running
```bash
# Basic command-line run (assuming you named the project raytracer)
./raytracer

# Or using Make, which produces output ImageMaker
make run

# To validate the image is as expected either open with a program that supports P6 PNM
gimp image2.ppm

# or convert to a PNG
convert image2.ppm image.png

# or use my awful Makefile if you wish to
make validate
```

### Usage Example
Modify the switch block in main.cpp to select whatever default scene you want:
```cpp
int main() {
    switch (num) {
        case 1: bouncing_spheres(); break;
        case 2: checkered_spheres(); break;
        case 3: earth(); break;
        case 4: perlin_spheres(); break;
        case 5: quads(); break;
        case 6: simple_light(); break;
        case 7: cornell_box(); break;
        case 8: cornell_smoke(); break;
        case 9: final_scene(800, 10000, 40); break;
        case 21: standard_spheres(); break;
        default: final_scene(400, 250, 4); break;
    }
}
```

Or, make your own scene! It's easy with C++:
```cpp
// near top of file:
void render(camera&, hittable_list&);
void final_scene(int, int, int);
void your_scene(); // Or declare it here. I'm not your employer.

// At main:

int main() {
    switch (yourNum) {
        case 1: bouncing_spheres(); break;
        // ...
        case yourNum: your_scene(); break;
    }
}

// At location of choice:
void your_scene() { // or whatever name you choose
    hittable_list world;

    // include declaration of the entities in this world, such as:
    // A lambertian sphere:
    world.add(make_shared<sphere>(
        point3(center_x, center_y, center_z),
        radius,
        make_shared<lambertian>(albedo)
    ));

    // A glass box, and by box I mean throwable brick:
    world.add(make_shared<box>(
        point3(x, y, z),
        point3(sizex, sizey, sizez),
        make_shared<dielectric>(1.5)
        ));
    
    // Earth
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);
    world.add(globe);

    // Uhh... the God Sphere:
    auto difflight = make_shared<diffuse_light>(color(6000,6000,6000));
    world.add(make_shared<sphere>(point3(0, 1000, 0), 2, difflight));

    // Homophobic Miku Sphere (the image is there because
    // Firefox downloaded the image to this project file somehow,
    // strange that I need to start a server to open HTMLs but
    // Firefox can just download to that file just fine):
    auto miku = make_shared<image_texture>("yea you can stay.png");
    auto migu_surface = make_shared<lambertian>(miku);
    auto LackOfEmployment = make_shared<sphere>(point3(0,0,0), 2, migu_surface);
    world.add(LackOfEmployment); // yeah you kinda just replace the names it's not that hard

    // And then you can declare the camera and its parameters:
    camera cam;

    cam.aspectratio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 5;
    cam.background = color(0.7, 0.8, 1.l);

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    // And then render:
    render(cam, world);
}
```
## Notes
BVH accelerates render time from an O(n) process to O(log n) process. In theory, this should result in a considerable speedup, and it does, although this is a bit hindered by Stuff(TM) meaning for heavy renders this may still take a while.

Rendering time is dependent on:
* Chosen resolution
* Chosen antialiasing quality (samples per pixel)
* Max ray bounce depth
* The amount of objects
* The speed of the processor
* The quality of the compiler
* Mercury's movement across the firmament
* The amount of cinnabar I've consumed on a given day
* Luck

With my system (ThinkPad T14 Gen2 w/Intel Core i7-1185G7) with case 42 (default), the render takes 58.066 seconds. This is variable depending on how many cores you have, choice of optimizations (for these I run with `-O3`), the quality of optimizations, and the aforementioned rendering time dependencies (it's literally just that; I'm not gonna repeat myself; there's no minimum word count I'm trying to reach here).

## References
[_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley, Trevor David Black, and Steve Hollasch.

[_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html) by Peter Shirley, Trevor David Black, and Steve Hollasch.

## License
Just to cover my ass, this project is available under the [MIT License](LICENSE)

## Acknowledgements
All code derived from Shirley et al's *Ray Tracing in One Weekend* and *Ray Tracing: The Next Week*. Other code stolen from probably Stack Overflow or such. If desired, this may be used to further experiment with ray tracing or as a starting platform for further implementations.


Note: This bad CPU-based ray tracer is CPU-based. In a production environment please use something better, like many of the hundreds of projects that are GPU-accelerated.