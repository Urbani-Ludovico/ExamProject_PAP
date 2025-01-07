//
// Ludovico Urbani <SM3201372>
//

#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <stdint.h>

#include "scene/scene.h"
#include "tools/argv_tools.h"

/**
 * Raytrace function
 * @param map mmap object where draw image
 * @param scene Scene object
 * @param image_width Image width
 * @param image_height Image height
 */
void raytrace(uint8_t* map, Scene * scene, image_size_t image_width, image_size_t image_height);

#endif
