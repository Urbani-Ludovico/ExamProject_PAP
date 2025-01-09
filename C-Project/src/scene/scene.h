//
// Ludovico Urbani <SM3201372>
//

#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

struct _SphereColor {
    uint8_t color_red;
    uint8_t color_green;
    uint8_t color_blue;
};

typedef struct _SphereColor SphereColor;

#ifdef __AVX2__
// If intrinsics available I use arrays of coordinates insted of array of structs
struct _Scene {
    float viewport_x;
    float viewport_y;
    float viewport_z;

    unsigned int sphere_count;
    unsigned int sphere_total_count;
    float* sphere_x;
    float* sphere_y;
    float* sphere_z;
    float* sphere_r;
    SphereColor* sphere_colors;

    uint8_t background_red;
    uint8_t background_green;
    uint8_t background_blue;
};

typedef struct _Scene Scene;

#else
struct _Sphere {
    float x;
    float y;
    float z;
    float radius;
};

typedef struct _Sphere Sphere;
struct _Scene {
    float viewport_x;
    float viewport_y;
    float viewport_z;

    unsigned int sphere_count;
    Sphere* spheres;
    SphereColor* sphere_colors;

    uint8_t background_red;
    uint8_t background_green;
    uint8_t background_blue;
};

typedef struct _Scene Scene;

#endif

/**
 * Allocate scene object.
 * Set array of objects to empty.
 * @param scene Pointer to variable where store Scene object pointer
 * @return Error code
 */
int scene_init(Scene** scene);

/**
 * Safe destroy scene object
 * @param scene Allocated scene object
 */
void scene_destroy(Scene* scene);

#endif
