//
// Ludovico Urbani <SM3201372>
//

#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

struct _SceneObject {
    float x;
    float y;
    float z;
    float radius;

    uint8_t color_red;
    uint8_t color_green;
    uint8_t color_blue;
};

typedef struct _SceneObject* SceneObject;

struct _Scene {
    float viewport_x;
    float viewport_y;
    float viewport_z;

    uint8_t background_red;
    uint8_t background_green;
    uint8_t background_blue;

    unsigned int objects_count;
    SceneObject* objects;
};

typedef struct _Scene* Scene;

Scene scene_init();
void scene_destroy(Scene scene);

#endif
