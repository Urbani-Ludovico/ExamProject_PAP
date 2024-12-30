//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../tools/argv_tools.h"
#include "../tools/logs.h"

#include "ppm.h"


unsigned int ppm_init(const char* out_path, const image_size_t width, const image_size_t height, FILE** file_out, uint8_t ** data_out) {
    printf(LOG_STEP("Opening ppm file"));

    FILE* file = fopen(out_path, "wb");
    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        exit(4);
    }

    if (fprintf(file, "P6\n%d %d\n255\n", width, height) < 0) {
        printf(LOG_ERROR("Write Error", "Error while writing data to file."));
        exit(8);
    }
    fflush(file);

    const int fd = fileno(file);

    if (ftruncate(fd, width * height * 3 + ftell(file)) < 0) {
        printf(LOG_ERROR("Trucate Error", "ftruncate on ppm file returns and error."));
        exit(7);
    }

    uint8_t * data = mmap(NULL, width * height * 3 * sizeof(uint8_t), PROT_WRITE, MAP_SHARED, fd, ftell(file));
    if (data == MAP_FAILED) {
        printf(LOG_ERROR("Memomry Map Failed", "Function mmap returns a MAP_FAILED."));
        exit(9);
    }

    *file_out = file;
    *data_out = data;

    return width * height * 3 + ftell(file);
}

void ppm_end(FILE* file, uint8_t * data, const unsigned int data_size) {
    printf(LOG_STEP("Closing ppm file"));
    munmap(data, data_size * sizeof(uint8_t));
    fclose(file);
}