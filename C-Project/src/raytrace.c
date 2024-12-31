//
// Ludovico Urbani <SM3201372>
//

#include <math.h>
#include <omp.h>
#include <stdio.h>

#include "scene/scene.h"
#include "tools/argv_tools.h"

#include "raytrace.h"


void raytrace([[maybe_unused]] uint8_t* map, const Scene scene, const image_size_t image_width, const image_size_t image_height) {
    #pragma omp parallel for schedule(dynamic, 1)
    for (image_size_t y = 0; y < image_height; y++) {
        const double vy = scene->viewport_y * (double)y / (image_height - 1.0) - scene->viewport_y / 2.0;

        for (image_size_t x = 0; x < image_width; x++) {
            // Vector
            const double vx = scene->viewport_x * (double)x / (image_width - 1.0) - scene->viewport_x / 2.0;

            // Unitary vector
            const double norm = sqrt(vx * vx + vy * vy + scene->viewport_z * scene->viewport_z);
            const double vx1 = vx / norm;
            const double vy1 = vy / norm;
            const double vz1 = scene->viewport_z / norm;
        }
    }
}
