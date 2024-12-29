
COMPILER = gcc
COMPILER_ERROR_FLAGS = -Wall -Wextra -Wpedantic -Werror
COMPILER_FLAGS = -O3 -std=c18 ${COMPILER_ERROR_FLAGS}

all: build/main

build/main: build/includes/main.o
	@ cp $< build/main
	@ chmod ugo+x build/main

build/includes/main.o: src/main.c
	${COMPILER} ${COMPILER_FLAGS} $< -o $@

.PHONY: clean
clean:
	@ find build/ -type f -not -name ".gitignore" -exec rm {} +
	@ echo Clean project!