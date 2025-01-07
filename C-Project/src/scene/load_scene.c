//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "scene.h"
#include "../tools/logs.h"

#include "load_scene.h"


Scene * load_scene(const char* scene_path) {
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
    Scene * scene = scene_init();

    //
    // Viewport
    //
    printf(LOG_STEP("Loading scene parameters"));

    if (fscanf(file, "VP %f %f %f\n", &scene->viewport_x, &scene->viewport_y, &scene->viewport_z) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read viewport data at line 1"));
        exit(5);
    }
    printf("\tViewport: %f x %f, distance = %f\n", scene->viewport_x, scene->viewport_y, scene->viewport_z);

    //
    // Background color
    //
    unsigned int r, g, b; // Use tmp variables for rgb color, because fscanf returns unsigned int and I need to cast them to uint8_t.
    if (fscanf(file, "BG %u %u %u\n", &r, &g, &b) != 3) {
        printf(LOG_ERROR("Malformed scene file", "Can not read background data at line 2"));
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
    if (fscanf(file, "OBJ_N %u\n", &scene->objects_count) != 1) {
        printf(LOG_ERROR("Malformed scene file", "Can not read objects number at line 3"));
        exit(5);
    }
    printf("\tObjects count: %u\n", scene->objects_count);

    //
    // Objects
    //
    printf(LOG_STEP("Loading scene objects"));

    scene->objects = (SceneObject *)calloc(scene->objects_count, sizeof(struct _SceneObject));
    if (scene->objects == NULL) {
        printf(LOG_ERROR("Allocation error", "Failed to allocate memory for scene objects."));
        exit(6);
    }

    for (unsigned int i = 0; i < scene->objects_count; i++) {
        if (fscanf(file, "S %f %f %f %f %u %u %u\n", &scene->objects[i].x, &scene->objects[i].y, &scene->objects[i].z, &scene->objects[i].radius, &r, &g, &b) != 7) {
            printf(LOG_ERROR("Malformed scene file", "Can not read object %u."), i + 1);
            exit(5);
        }
        // Same code logic as in background code.
        scene->objects[i].color_red = r <= 255 ? (uint8_t)r : 255;
        scene->objects[i].color_green = g <= 255 ? (uint8_t)g : 255;
        scene->objects[i].color_blue = b <= 255 ? (uint8_t)b : 255;
    }

    fclose(file);

    return scene;
}
