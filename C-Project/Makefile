
COMPILER = gcc
COMPILER_ERROR_FLAGS = -Wall -Wextra -Wpedantic -Werror
COMPILER_FLAGS = -O3 -std=gnu2x ${COMPILER_ERROR_FLAGS}

#
# COMPILE
#

all: build/main

build/main: build/includes/main.o build/includes/argv_tools.o build/includes/load_scene.o build/includes/ppm.o build/includes/scene.o
	${COMPILER} $^ -o $@

build/includes/argv_tools.o: src/tools/argv_tools.c src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/load_scene.o: src/scene/load_scene.c src/scene/load_scene.h src/scene/scene.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/main.o: src/main.c src/image/ppm.h src/scene/load_scene.h src/scene/scene.h src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/ppm.o: src/image/ppm.c src/image/ppm.h src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/scene.o: src/scene/scene.c src/scene/scene.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

#
# TESTS
#

.PHONY: test
test: t1

.PHONY: t1
t1: build/main tests/t1.txt
	@ echo ""
	@ echo "- - - Test 1 - - -"
	@ echo ""
	@ build/main tests/t1.txt tests/t1_1920x1080.ppm 1920 1080

#
# CLEAN
#

.PHONY: clean
clean:
	@ find "build/" -type f -not -name ".gitignore" -exec rm {} \;
	@ echo Clean project!