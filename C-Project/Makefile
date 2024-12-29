
COMPILER = gcc
COMPILER_ERROR_FLAGS = -Wall -Wextra -Wpedantic -Werror
COMPILER_FLAGS = -O3 -std=c18 ${COMPILER_ERROR_FLAGS}

all: build/main

build/main: build/includes/main.o build/includes/argv_tools.o
	${COMPILER} $^ -o $@

build/includes/main.o: src/main.c build/includes/argv_tools.o build/includes/load_scene.o build/includes/scene.o
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/scene.o: src/scene/scene.c src/scene/scene.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/load_scene.o: src/scene/load_scene.c src/scene/load_scene.h build/includes/scene.o src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

build/includes/argv_tools.o: src/tools/argv_tools.c src/tools/argv_tools.h src/tools/logs.h
	${COMPILER} -c ${COMPILER_FLAGS} $< -o $@

.PHONY: clean
clean:
	@ find build/ -type f -not -name ".gitignore" -exec rm {} +
	@ echo Clean project!