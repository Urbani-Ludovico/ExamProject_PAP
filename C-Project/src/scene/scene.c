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

    (*scene)->objects = NULL;
    (*scene)->objects_count = 0;

    return 0;
}


void scene_destroy(Scene* scene) {
    printf(LOG_STEP("Destroying scene"));
    if (scene != NULL) {
        if (scene->objects != NULL) {
            free(scene->objects);
        }

        free(scene);
    }
}
