//
// Ludovico Urbani <SM3201372>
//

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "scene/scene.h"
#include "tools/argv_tools.h"
#include "tools/logs.h"

#include "raytrace.h"


#ifndef DOUBLE_TOLLERANCE
#define DOUBLE_TOLLERANCE 1e-6
#endif

// SPHERE_CHECK_DISTANCE use in if a branchless absolute value
#ifndef SPHERE_CHECK_DISTANCE
#define SPHERE_CHECK_DISTANCE(equation) \
    distance = equation; \
    if (distance * ((distance > 0) - (distance < 0)) < min_distance) { \
        min_distance = distance; \
        color_red = sphere.color_red; \
        color_green = sphere.color_green; \
        color_blue = sphere.color_blue; \
    }
#endif

void raytrace(uint8_t* map, Scene * scene, const image_size_t image_width, const image_size_t image_height) {
    printf(LOG_STEP("Raytrace started"));

    // Parallelize only rows of image. Because, in the rappresentation of image, a row is a contiguous subarray of mmap. So having a contiguous array (row) in same thread helps branch prediction
    #pragma omp parallel for schedule(dynamic, 1)
    for (image_size_t y = 0; y < image_height; y++) {
        const unsigned int row = (image_height - y - 1) * image_width * 3;

        // Vector y (same for alla row)
        const double vy = scene->viewport_y * (double)y / (image_height - 1.0) - scene->viewport_y / 2.0;

        for (image_size_t x = 0; x < image_width; x++) {
            // Prefetch next index of mmap
            if (x < image_width - 1) {
                __builtin_prefetch(&map[row + (x + 1) * 3]);
                __builtin_prefetch(&map[row + (x + 1) * 3 + 1]);
                __builtin_prefetch(&map[row + (x + 1) * 3 + 2]);
            }

            // Vector x
            const double vx = scene->viewport_x * (double)x / (image_width - 1.0) - scene->viewport_x / 2.0;

            // Unitary vector
            const double norm = sqrt(vx * vx + vy * vy + scene->viewport_z * scene->viewport_z);
            const double vx1 = vx / norm;
            const double vy1 = vy / norm;
            const double vz1 = scene->viewport_z / norm;

            // Object distance and color for sphere with min distance
            double min_distance = DBL_MAX;
            uint8_t color_red = scene->background_red;
            uint8_t color_green = scene->background_green;
            uint8_t color_blue = scene->background_blue;

            // a, b, c and delta are the parameters of the formula for second degree equation
            const double a = vx1 * vx1 + vy1 * vy1 + vz1 * vz1;
            double distance;
            for (unsigned int s = 0; s < scene->objects_count; s++) {
                // Prefetch next sphere object
                if (s < scene->objects_count - 1) {
                    __builtin_prefetch(&scene->objects[s + 1]);
                }
                const SceneObject sphere = scene->objects[s];

                const double b = -2.0 * (sphere.x * vx1 + sphere.y * vy1 + sphere.z * vz1);
                const double c = sphere.x * sphere.x + sphere.y * sphere.y + sphere.z * sphere.z - sphere.radius * sphere.radius;

                const double delta = b * b - 4 * a * c;
                // Considering floating point errors I use abs(delta) < epsilow instead of delta == 0
                // I expect that it's greather the number of sphere that do not intersect ray
                if (__builtin_expect(delta > -DOUBLE_TOLLERANCE && delta < DOUBLE_TOLLERANCE, 0)) {
                    SPHERE_CHECK_DISTANCE(-b - 2 * a);
                } else if (__builtin_expect(delta > 0, 0)) {
                    SPHERE_CHECK_DISTANCE((-b - sqrt(delta)) / (2 * a))
                    SPHERE_CHECK_DISTANCE((-b + sqrt(delta)) / (2 * a))
                }
            }

            map[row + x * 3] = color_red;
            map[row + x * 3 + 1] = color_green;
            map[row + x * 3 + 2] = color_blue;
        }
    }
}
