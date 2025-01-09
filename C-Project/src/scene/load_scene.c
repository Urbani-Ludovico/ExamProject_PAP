//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "scene.h"
#include "../tools/logs.h"

#include "load_scene.h"


Scene* load_scene(const char* scene_path) {
    //
    // Load file
    //
    printf(LOG_STEP("Opening scene file"));
    FILE* file = fopen(scene_path, "r");

    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        exit(4);
    }

    //
    // Create scene
    //
    Scene* scene;
    const int scene_error = scene_init(&scene);
    if (scene_error != 0) {
        fclose(file);
        exit(scene_error);
    }

    //
    // Viewport
    //
    printf(LOG_STEP("Loading scene parameters"));

    if (fscanf(file, "VP %f %f %f\n", &scene->viewport_x, &scene->viewport_y, &scene->viewport_z) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read viewport data at line 1"));
        fclose(file);
        scene_destroy(scene);
        exit(5);
    }
    printf("\tViewport: %f x %f, distance = %f\n", scene->viewport_x, scene->viewport_y, scene->viewport_z);

    //
    // Background color
    //
    unsigned int r, g, b; // Use tmp variables for rgb color, because fscanf returns unsigned int and I need to cast them to uint8_t.
    if (fscanf(file, "BG %u %u %u\n", &r, &g, &b) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read background data at line 2"));
        fclose(file);
        scene_destroy(scene);
        exit(5);
    }
    // Color greather than 255 is interpreted as 255
    scene->background_red = r <= 255 ? (uint8_t)r : 255;
    scene->background_green = g <= 255 ? (uint8_t)g : 255;
    scene->background_blue = b <= 255 ? (uint8_t)b : 255;
    printf("\tBackground RGB: %u, %u, %u\n", scene->background_red, scene->background_green, scene->background_blue);

    //
    // Objects count
    //
    if (fscanf(file, "OBJ_N %u\n", &scene->sphere_count) != 1) {
        printf(LOG_ERROR("Malformed scene file", "Can not read objects number at line 3"));
        fclose(file);
        scene_destroy(scene);
        exit(5);
    }
    printf("\tObjects count: %u\n", scene->sphere_count);

    //
    // Objects
    //
    printf(LOG_STEP("Loading scene objects"));

    #ifdef __AVX2__
    // If intrinsics available array of coordinates are multiple of 8
    // With this method I can load bloacks of 8 floats into vector instructions (empty cells are computed, but value will never read. This empty cells does not cost much in my opinion)
    scene->sphere_total_count = scene->sphere_count % 8 == 0 ? scene->sphere_count : (8 * ((scene->sphere_count + 8) / 8));
    scene->sphere_x = (float*)calloc(scene->sphere_total_count, sizeof(float));
    scene->sphere_y = (float*)calloc(scene->sphere_total_count, sizeof(float));
    scene->sphere_z = (float*)calloc(scene->sphere_total_count, sizeof(float));
    scene->sphere_r = (float*)calloc(scene->sphere_total_count, sizeof(float));
    if (scene->sphere_x == NULL || scene->sphere_y == NULL || scene->sphere_z == NULL || scene->sphere_r == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for sphere coordinates."));
        fclose(file);
        scene_destroy(scene);
        exit(6);
    }
    #else
    scene->spheres = (Sphere *)calloc(scene->sphere_count, sizeof(struct _Sphere));
    if (scene->spheres == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene objects."));
        fclose(file);
        scene_destroy(scene);
        exit(6);
    }
    #endif

    scene->sphere_colors = (SphereColor*)calloc(scene->sphere_count, sizeof(struct _SphereColor));
    if (scene->sphere_colors == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene object colors."));
        fclose(file);
        scene_destroy(scene);
        exit(6);
    }

    for (unsigned int i = 0; i < scene->sphere_count; i++) {
        #ifdef __AVX2__
        if (fscanf(file, "S %f %f %f %f %u %u %u\n", &scene->sphere_x[i], &scene->sphere_y[i], &scene->sphere_z[i], &scene->sphere_r[i], &r, &g, &b) != 7) {
            printf(LOG_ERROR("Malformed scene file", "Can not read object %u."), i + 1);
            fclose(file);
            scene_destroy(scene);
            exit(5);
        }
        #else
        if (fscanf(file, "S %f %f %f %f %u %u %u\n", &scene->spheres[i].x, &scene->spheres[i].y, &scene->spheres[i].z, &scene->spheres[i].radius, &r, &g, &b) != 7) {
            printf(LOG_ERROR("Malformed scene file", "Can not read object %u."), i + 1);
            fclose(file);
            scene_destroy(scene);
            exit(5);
        }
        #endif

        // Same code logic as in background code.
        scene->sphere_colors[i].color_red = r <= 255 ? (uint8_t)r : 255;
        scene->sphere_colors[i].color_green = g <= 255 ? (uint8_t)g : 255;
        scene->sphere_colors[i].color_blue = b <= 255 ? (uint8_t)b : 255;
    }

    fclose(file);

    return scene;
}
