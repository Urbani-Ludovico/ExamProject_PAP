//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "../tools/logs.h"

#include "load_scene.h"

Scene load_scene(const char* scene_path) {
    printf(LOG_STEP("Opening scene file"));
    FILE* file = fopen(scene_path, "r");

    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        exit(4);
    }

    const Scene scene = scene_init();

    printf(LOG_STEP("Loading scene parameters"));

    if (fscanf(file, "VP %f %f %f\n", &scene->viewport_x, &scene->viewport_y, &scene->viewport_z) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read viewport data at line 1"));
        scene_destroy(scene);
        exit(5);
    }
    printf("\tViewport: %f x %f, distance = %f\n", scene->viewport_x, scene->viewport_y, scene->viewport_z);

    unsigned int r, g, b;
    if (fscanf(file, "BG %u %u %u\n", &r, &g, &b) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read background data at line 2"));
        scene_destroy(scene);
        exit(5);
    }
    scene->background_red = (uint8_t)r;
    scene->background_green = (uint8_t)g;
    scene->background_blue = (uint8_t)b;
    printf("\tBackground RGB: %u, %u, %u\n", scene->background_red, scene->background_green, scene->background_blue);

    if (fscanf(file, "OBJ_N %u\n", &scene->objects_count) != 1) {
        printf(LOG_ERROR("Malformed scene file", "Can not read objects number at line 3"));
        scene_destroy(scene);
        exit(5);
    }
    printf("\tObjects count: %u\n", scene->objects_count);

    printf(LOG_STEP("Loading scene objects"));

    scene->objects = (SceneObject*)calloc(scene->objects_count, sizeof(SceneObject));
    if (scene->objects == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene objects."));
        scene_destroy(scene);
        exit(6);
    }

    for (unsigned int i = 0; i < scene->objects_count; i++) {
        scene->objects[i] = (SceneObject)malloc(sizeof(struct _SceneObject));
        if (scene->objects[i] == NULL) {
            printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene object %u."), i + 1);
            scene_destroy(scene);
            exit(6);
        }

        if (fscanf(file, "S %f %f %f %f %u %u %u\n", &scene->objects[i]->x, &scene->objects[i]->y, &scene->objects[i]->z, &scene->objects[i]->radius, &r, &g, &b) != 7) {
            printf(LOG_ERROR("Malformed scene file", "Can not read object %u."), i + 1);
            scene_destroy(scene);
            exit(5);
        }
        scene->objects[i]->color_red = (uint8_t)r;
        scene->objects[i]->color_green = (uint8_t)g;
        scene->objects[i]->color_blue = (uint8_t)b;
        // printf("\tSphere: x=%.3f, y=%.3f, z=%.3f, r=%.3f, bg=(%u, %u, %u)\n", scene->objects[i]->x, scene->objects[i]->y, scene->objects[i]->z, scene->objects[i]->radius, scene->objects[i]->color_red, scene->objects[i]->color_green, scene->objects[i]->color_blue);
    }

    fclose(file);

    return scene;
}
