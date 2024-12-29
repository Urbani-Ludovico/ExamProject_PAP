//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>

#include "load_scene.h"

#include "../tools/logs.h"


Scene load_scene(const char * scene_path) {
    FILE * file = fopen(scene_path, "r");

    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        exit(4);
    }

    const Scene scene = (Scene)malloc(sizeof(struct _Scene));

    fclose(file);

    return scene;
}
