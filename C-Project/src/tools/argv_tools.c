//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>
#include <stdlib.h>

#include "logs.h"

#include "argv_tools.h"


image_size_t argv_parse_size(const char* str) {
    const long int tmp =  strtol(str, NULL, 0);
    if (tmp <= 0) {
        printf(LOG_ERROR("Value error", "Size is not valid, expected uint32"));
        exit(3);
    }
    return (image_size_t)tmp;
}

inline void argv_missing_arg() {
    printf(LOG_ERROR("Missing arguments", "The program needs 4 arguments:\n\t- [char *] scene_file.txt\n\t- [char *] out_file.ppm\n\t- [uint32] image_width\n\t- [uint32] image_height"));
    exit(2);
}