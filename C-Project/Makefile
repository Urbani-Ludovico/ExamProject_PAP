
COMPILER = gcc
COMPILER_ERROR_FLAGS = -Wall -Wextra -Wpedantic -Werror
COMPILER_FLAGS = -O3 -std=c18 ${COMPILER_ERROR_FLAGS}

BUILD_PATH = ./build/
PARTIAL_BUILD_PATH = ${BUILD_PATH}includes/

.PHONY: clean
clean:
	@ find ${BUILD_PATH} -type f -not -name ".gitignore" -exec rm {} +
	@ echo Clean project!