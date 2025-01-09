//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>

#include "../tools/logs.h"

#include "scene.h"


int scene_init(Scene** scene) {
    printf(LOG_STEP("Initializing scene"));
    *scene = (Scene*)malloc(sizeof(Scene));

    if (*scene == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene.\n"));
        return 6;
    }

    #ifdef __AVX2__
    (*scene)->sphere_total_count = 0;
    (*scene)->sphere_x = NULL;
    (*scene)->sphere_y = NULL;
    (*scene)->sphere_z = NULL;
    (*scene)->sphere_r = NULL;
    #else
    (*scene)->spheres = NULL;
    #endif

    (*scene)->sphere_count = 0;

    return 0;
}


void scene_destroy(Scene* scene) {
    printf(LOG_STEP("Destroying scene"));
    if (scene != NULL) {
        #ifdef __AVX2__
        if (scene->sphere_x != NULL) {
            free(scene->sphere_x);
        }
        if (scene->sphere_y != NULL) {
            free(scene->sphere_y);
        }
        if (scene->sphere_z != NULL) {
            free(scene->sphere_z);
        }
        if (scene->sphere_r != NULL) {
            free(scene->sphere_r);
        }
        #else
        if (scene->spheres != NULL) {
            free(scene->spheres);
        }
        #endif

        if (scene->sphere_colors != NULL) {
            free(scene->sphere_colors);
        }

        free(scene);
    }
}
