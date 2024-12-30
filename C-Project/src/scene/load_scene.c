//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>

#include "load_scene.h"

#include "../tools/logs.h"


Scene load_scene(const char* scene_path) {
    printf(LOG_STEP("Opening scene file"));
    FILE* file = fopen(scene_path, "r");

    printf(LOG_STEP("Loading scene parameters"));

    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        exit(4);
    }

    const Scene scene = scene_init();

    if (fscanf(file, "VP %f %f %f\n", &scene->viewport_x, &scene->viewport_y, &scene->viewport_z) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read viewport data at line 1"));
        exit(5);
    }
    printf("\tViewport: width = %f, height = %f, distance = %f\n", scene->viewport_x, scene->viewport_y, scene->viewport_z);

    unsigned int r, g, b;
    if (fscanf(file, "BG %u %u %u\n", &r, &g, &b) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read background data at line 2"));
        exit(5);
    }
    scene->background_red = (uint8_t)r;
    scene->background_green = (uint8_t)g;
    scene->background_blue = (uint8_t)b;
    printf("\tBackground color: red = %u, green = %u, blue = %u\n", scene->background_red, scene->background_green, scene->background_blue);

    if (fscanf(file, "OBJ_N %u\n", &scene->objects_count) != 1) {
        printf(LOG_ERROR("Malformed scene file", "Can not read objects number at line 3"));
        exit(5);
    }
    printf("\tObjects count: %u\n", scene->objects_count);

    printf(LOG_STEP("Loading scene objects"));

    scene->objects = (SceneObject*)malloc(sizeof(struct _SceneObject) * scene->objects_count);
    if (scene->objects == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene objects.\n"));
        exit(6);
    }

    for (unsigned int i = 0; i < scene->objects_count; i++) {
        if (fscanf(file, "S %f %f %f %f %u %u %u\n", &scene->objects[i]->x, &scene->objects[i]->y, &scene->objects[i]->z, &scene->objects[i]->radius, &r, &g, &b) != 7) {
            printf(LOG_ERROR("Malformed scene file", "Can not read object %u\n"), i);
            exit(5);
        }
        scene->objects[i]->background_red = (uint8_t)r;
        scene->objects[i]->background_green = (uint8_t)g;
        scene->objects[i]->background_blue = (uint8_t)b;
        printf("\tSphere: x = %f, y = %f, z = %f, radius = %f, background[RGB] = (%u, %u, %u)\n", scene->objects[i]->x, scene->objects[i]->y, scene->objects[i]->z, scene->objects[i]->radius, scene->objects[i]->background_red, scene->objects[i]->background_green, scene->objects[i]->background_blue);
    }

    fclose(file);

    return scene;
}
