
COMPILER = gcc
COMPILER_ERROR_FLAGS = -Wall -Wextra -Wpedantic -Werror
COMPILER_EXTENSIONS = -fopenmp -mno-avx2
COMPILER_FLAGS = -O3 -std=gnu2x -march=native ${COMPILER_ERROR_FLAGS} ${COMPILER_EXTENSIONS}

#
# COMPILE
#

all: build/main

build/main: build/includes/main.o build/includes/argv_tools.o build/includes/load_scene.o build/includes/ppm.o build/includes/raytrace.o build/includes/scene.o
	${COMPILER} ${COMPILER_FLAGS} $^ -o $@ -lm

build/includes/argv_tools.o: src/tools/argv_tools.c src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/load_scene.o: src/scene/load_scene.c src/scene/load_scene.h src/scene/scene.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/main.o: src/main.c src/image/ppm.h src/raytrace.h src/scene/load_scene.h src/scene/scene.h src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/ppm.o: src/image/ppm.c src/image/ppm.h src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/raytrace.o: src/raytrace.c src/raytrace.h src/scene/scene.h src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@ -lm

build/includes/scene.o: src/scene/scene.c src/scene/scene.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

#
# TESTS
#

test: t1a t1b t1c

.PHONY: t1a
t1a: build/main tests/t1.txt
	@ echo ""
	@ echo "- - - Test 1-A - - -"
	@ echo ""
	@ build/main tests/t1.txt tests/t1_1920x1080.ppm 1920 1080

.PHONY: t1b
t1b: build/main tests/t1.txt
	@ echo ""
	@ echo "- - - Test 1-B - - -"
	@ echo ""
	@ build/main tests/t1.txt tests/t1_144x81.ppm 144 81

.PHONY: t1c
t1c: build/main tests/t1.txt
	@ echo ""
	@ echo "- - - Test 1-C - - -"
	@ echo ""
	@ build/main tests/t1.txt tests/t1_7680x4320.ppm 7680 4320

#
# CLEAN
#

.PHONY: clean
clean:
	@ find "build/" -type f -not -name ".gitignore" -exec rm {} \;
	@ echo Clean project!