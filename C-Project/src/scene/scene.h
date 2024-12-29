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

    uint8_t background_red;
    uint8_t background_green;
    uint8_t background_blue;
};
typedef struct _SceneObject * SceneObject;

struct _Scene {
    float viewport_x;
    float viewport_y;
    float viewport_z;

    uint8_t background_red;
    uint8_t background_green;
    uint8_t background_blue;

    uint16_t objects_count;
    SceneObject * objects;
};
typedef struct _Scene * Scene;

#endif
