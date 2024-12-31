//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>

#include "../tools/logs.h"

#include "scene.h"


Scene * scene_init() {
    printf(LOG_STEP("Initializing scene"));
    auto const scene = (Scene *)malloc(sizeof(Scene));

    if (scene == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene.\n"));
        exit(6);
    }

    scene->objects = NULL;
    scene->objects_count = 0;

    return scene;
}


void scene_destroy(Scene * scene) {
    printf(LOG_STEP("Destroying scene"));
    if (scene != NULL) {
        if (scene->objects != NULL) {
            free(scene->objects);
        }

        free(scene);
    }
}
