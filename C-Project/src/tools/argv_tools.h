//
// Ludovico Urbani <SM3201372>
//

#ifndef ARGV_TOOLS_H
#define ARGV_TOOLS_H

#include <stdint.h>

typedef uint32_t image_size_t;

/**
 * Parse image size from argv string.
 * Check if argument can be cast in image_size_t type
 * @param str Argument from argv
 * @return Image size if possible
 */
image_size_t argv_parse_size(const char * str);

void argv_missing_arg();

#endif
