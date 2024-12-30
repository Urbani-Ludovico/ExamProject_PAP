//
// Ludovico Urbani <SM3201372>
//

#ifndef PPM_H
#define PPM_H

#include "../tools/argv_tools.h"

typedef unsigned char * ppm_t;

void ppm_init(const char* out_path, image_size_t width, image_size_t height, FILE** file_out);
void ppm_end(FILE* file);

#endif
