//
// Ludovico Urbani <SM3201372>
//

#include <stdio.h>

#include "tools/argv_tools.h"
#include "tools/logs.h"

int main(const int argc, const char * argv[]) {
    printf(LOG_STEP("Starting program"));

    if (argc < 5) {
        argv_missing_arg();
    }

    const char * scene_path = argv[1];
    const char * out_path = argv[2];
    const image_size_t image_width = argv_parse_size(argv[3]);
    const image_size_t image_height = argv_parse_size(argv[4]);

    printf("%s\n%s\n%u\n%u\n", scene_path, out_path, image_width, image_height);

    return 0;
}