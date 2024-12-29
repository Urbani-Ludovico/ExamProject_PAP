//
// Ludovico Urbani <SM3201372>
//

#ifndef ARGV_TOOLS_H
#define ARGV_TOOLS_H

#include <stdint.h>

typedef uint32_t image_size_t;

image_size_t argv_parse_size(const char * str);

void argv_missing_arg();

#endif
