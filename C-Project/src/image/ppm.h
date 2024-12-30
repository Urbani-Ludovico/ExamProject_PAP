//
// Ludovico Urbani <SM3201372>
//

#ifndef PPM_H
#define PPM_H

#include "../tools/argv_tools.h"

typedef unsigned char* ppm_t;

size_t ppm_init(const char* out_path, image_size_t width, image_size_t height, FILE** file_out, uint8_t** data_out);
void ppm_end(FILE* file, uint8_t* data, size_t data_size);

#endif
