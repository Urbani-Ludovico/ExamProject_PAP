//
// Ludovico Urbani <SM3201372>
//

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __AVX2__
#include <immintrin.h>
#endif

#include "scene/scene.h"
#include "tools/argv_tools.h"
#include "tools/logs.h"

#include "raytrace.h"


#ifndef FLOAT_TOLLERANCE
#define FLOAT_TOLLERANCE 1e-6
#endif

// SPHERE_CHECK_DISTANCE use in if a branchless absolute value
#ifndef SPHERE_CHECK_DISTANCE
#define SPHERE_CHECK_DISTANCE(equation) \
    distance = equation; \
    if (__builtin_expect(distance * ((distance > 0) - (distance < 0)) < min_distance, 0)) { \
        min_distance = distance; \
        min_index = s; \
    }
#endif

typedef float v4f __attribute__((vector_size(16)));
typedef int v4i __attribute__((vector_size(16)));
v4i v4f_abs = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};

typedef float v8f __attribute__((vector_size(32)));
typedef int v8i __attribute__((vector_size(32)));
v8i v8f_abs = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};


void raytrace(uint8_t* map, Scene* scene, const image_size_t image_width, const image_size_t image_height) {
    printf(LOG_STEP("Raytrace started"));

    // Parallelize only rows of image. Because, in the rappresentation of image, a row is a contiguous subarray of mmap. So having a contiguous array (row) in same thread helps branch prediction
    #pragma omp parallel for schedule(static)
    for (image_size_t y = 0; y < image_height; y++) {
        const unsigned int row = (image_height - y - 1) * image_width * 3;

        // Vector y (same for alla row)
        const float vy = scene->viewport_y * (float)y / (float)(image_height - 1) - scene->viewport_y / 2;

        for (image_size_t x = 0; x < image_width; x++) {
            // Prefetch next index of mmap
            if (x < image_width - 1) {
                __builtin_prefetch(&map[row + (x + 1) * 3]);
                __builtin_prefetch(&map[row + (x + 1) * 3 + 1]);
                __builtin_prefetch(&map[row + (x + 1) * 3 + 2]);
            }

            // Vector x
            const float vx = scene->viewport_x * (float)x / (float)(image_width - 1) - scene->viewport_x / 2;

            // Unitary vector
            const float norm = (float)sqrt((double)(vx * vx + vy * vy + scene->viewport_z * scene->viewport_z));
            const float vx1 = vx / norm;
            const float vy1 = vy / norm;
            const float vz1 = scene->viewport_z / norm;

            float min_distance = FLT_MAX;
            unsigned int min_index = 0;

            // a, b, c and delta are the parameters of the formula for second degree equation
            const float a = vx1 * vx1 + vy1 * vy1 + vz1 * vz1;

            // Use SIMD optimization with AVX2 intrinsics (if available)
            #ifdef __AVX2__
            for (unsigned int sb = 0; sb < scene->sphere_total_count; sb += 8) {
                // Load data of 8 spheres into vector of 8 floats with intrinsics
                const __m256 x_vec = _mm256_loadu_ps(&scene->sphere_x[sb]);
                const __m256 y_vec = _mm256_loadu_ps(&scene->sphere_y[sb]);
                const __m256 z_vec = _mm256_loadu_ps(&scene->sphere_z[sb]);
                const __m256 r_vec = _mm256_loadu_ps(&scene->sphere_r[sb]);

                // 2 * (x * vx1 + y * vy1 + z * vz1); (with 2 insted of -2, I don't need to do -b in future)
                __m256 b = _mm256_mul_ps(_mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(x_vec, _mm256_set1_ps(vx1)), _mm256_mul_ps(y_vec, _mm256_set1_ps(vy1))), _mm256_mul_ps(z_vec, _mm256_set1_ps(vz1))), _mm256_set1_ps(2));
                // (x^2 + y^2) + (x^2 - r^y)
                __m256 c = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(x_vec, x_vec), _mm256_mul_ps(y_vec, y_vec)), _mm256_sub_ps(_mm256_mul_ps(z_vec, z_vec), _mm256_mul_ps(r_vec, r_vec)));
                // b^2 - 4 * a * c
                __m256 delta = _mm256_sub_ps(_mm256_mul_ps(b, b), _mm256_mul_ps(c, _mm256_set1_ps(4 * a)));

                // delta1 > -FLOAT_TOLLERANCE
                // Considering floating point errors I use abs(delta) < epsilow instead of delta == 0
                const __m256 delta_more_negative_tollerance = _mm256_cmp_ps(delta, _mm256_set1_ps(-FLOAT_TOLLERANCE), _CMP_GT_OS);
                // delta1 > FLOAT_TOLLERANCE
                const __m256 delta_more_tollerance = _mm256_cmp_ps(delta, _mm256_set1_ps(FLOAT_TOLLERANCE), _CMP_GE_OS);

                // Compute all distances
                // I use the bit mask _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF)) with all bit 1 except sign to make absolute value
                const __m256 distance_delta_zero = _mm256_and_ps(_mm256_div_ps(b, _mm256_set1_ps(2 * a)), _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF)));
                const __m256 distance_delta_positive1 = _mm256_and_ps(_mm256_div_ps(_mm256_sub_ps(b, _mm256_sqrt_ps(delta)), _mm256_set1_ps(2 * a)), _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF)));
                const __m256 distance_delta_positive2 = _mm256_and_ps(_mm256_div_ps(_mm256_add_ps(b, _mm256_sqrt_ps(delta)), _mm256_set1_ps(2 * a)), _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF)));

                // if(delta_more_tollerance, min(distance_delta_positive1, distance_delta_positive2), if(delta_more_negative_tollerance, distance_delta_zero, FLT_MAX))
                const __m256 distances_vect = _mm256_blendv_ps(_mm256_blendv_ps(_mm256_set1_ps(FLT_MAX), distance_delta_zero, delta_more_negative_tollerance), _mm256_min_ps(distance_delta_positive1, distance_delta_positive2), delta_more_tollerance);
                float distances[8];
                _mm256_storeu_ps(distances, distances_vect);

                // Iterate on 8 values of the array
                for (unsigned int i = 0; i < 8 && sb + i < scene->sphere_count; i++) {
                    // I expect that it's greather the number of sphere that do not intersect ray
                    if (__builtin_expect(distances[i] < min_distance, 0)) {
                        min_distance = distances[i];
                        min_index = sb + i;
                    }
                }
            }

            #else
            float distance;
            for (unsigned int s = 0; s < scene->sphere_count; s++) {
                // Prefetch next sphere object
                if (s < scene->sphere_count - 1) {
                    __builtin_prefetch(&scene->spheres[s + 1]);
                }
                const Sphere sphere = scene->spheres[s];

                const float b = 2 * (sphere.x * vx1 + sphere.y * vy1 + sphere.z * vz1);
                const float c = sphere.x * sphere.x + sphere.y * sphere.y + sphere.z * sphere.z - sphere.radius * sphere.radius;

                const float delta = b * b - 4 * a * c;
                // Considering floating point errors I use abs(delta) < epsilow instead of delta == 0
                // I expect that it's greather the number of sphere that do not intersect ray
                if (__builtin_expect(delta > FLOAT_TOLLERANCE, 0)) {
                    SPHERE_CHECK_DISTANCE((b - (float)sqrt((double)delta)) / (2 * a))
                    SPHERE_CHECK_DISTANCE((b + (float)sqrt((double)delta)) / (2 * a))
                } else if (__builtin_expect(delta > -FLOAT_TOLLERANCE, 0)) {
                    SPHERE_CHECK_DISTANCE(b / 2 * a)
                }
            }
            #endif

            if (min_distance != FLT_MAX) {
                map[row + x * 3] = scene->sphere_colors[min_index].color_red;
                map[row + x * 3 + 1] = scene->sphere_colors[min_index].color_green;
                map[row + x * 3 + 2] = scene->sphere_colors[min_index].color_blue;
            } else {
                map[row + x * 3] = scene->background_red;
                map[row + x * 3 + 1] = scene->background_green;
                map[row + x * 3 + 2] = scene->background_blue;
            }
        }
    }
}
