//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "../tools/argv_tools.h"
#include "../tools/logs.h"

#include "ppm.h"


void ppm_init(const char* out_path, const image_size_t width, const image_size_t height, FILE** file_out) {
    printf(LOG_STEP("Opening ppm file"));

    FILE* file = fopen(out_path, "wb");
    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        exit(4);
    }

    fprintf(file, "P6\n%d %d\n255\n", width, height);
    fflush(file);

    *file_out = file;
}

void ppm_end(FILE* file) {
    fclose(file);
}