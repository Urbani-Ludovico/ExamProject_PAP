//
// Ludovico Urbani <SM3201372>
//

#ifndef PPM_H
#define PPM_H

#include "../tools/argv_tools.h"

typedef unsigned char* ppm_t;

/**
 * Init mmap object for draw the image
 * @param out_path Path to save image
 * @param width Image width
 * @param height Image height
 * @param file_out Pointer to variable where store file object
 * @param data_out Pointer to variable where store mmap object
 * @param map_size Pointer to variable where store mmap size to use for close file
 * @return Error code
 */
int ppm_init(const char* out_path, image_size_t width, image_size_t height, FILE** file_out, uint8_t** data_out, size_t* map_size);

/**
 * Close mmap object
 * @param file File object
 * @param data mmap object
 * @param data_size mmap size
 */
void ppm_end(FILE* file, uint8_t* data, size_t data_size);

#endif
