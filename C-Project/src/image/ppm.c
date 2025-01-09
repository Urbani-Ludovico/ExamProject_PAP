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


int ppm_init(const char* out_path, const image_size_t width, const image_size_t height, FILE** file_out, uint8_t** data_out, size_t* map_size) {
    printf(LOG_STEP("Creating ppm file"));

    //
    // Open file
    //
    FILE* file = fopen(out_path, "w+b");
    if (file == NULL) {
        printf(LOG_ERROR("IO Error", "Unable to open file."));
        return 4;
    }

    //
    // Print image header
    //
    if (fprintf(file, "P6\n%d %d\n255\n", width, height) < 0) {
        printf(LOG_ERROR("Write Error", "Error while writing data to file."));
        fclose(file);
        return 8;
    }
    fflush(file);

    //
    // File descriptor
    //
    const int fd = fileno(file);
    const size_t file_size = width * height * 3 + sizeof(uint8_t) + ftell(file);

    //
    // Set file size with trucate
    //
    if (ftruncate(fd, (long int)file_size) < 0) {
        printf(LOG_ERROR("Trucate Error", "ftruncate on ppm file returns and error."));
        fclose(file);
        return 7;
    }

    //
    // Create mmap
    //
    uint8_t* data = mmap(NULL, file_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        printf(LOG_ERROR("Memory Map Failed", "Function mmap returns a MAP_FAILED."));
        fclose(file);
        return 9;
    }

    *file_out = file;
    *data_out = data + ftell(file);
    *map_size = file_size;

    return 0;
}


void ppm_end(FILE* file, uint8_t* data, const size_t data_size) {
    printf(LOG_STEP("Closing ppm file"));
    munmap(data, data_size * sizeof(uint8_t));
    fclose(file);
}
