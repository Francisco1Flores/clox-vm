#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl() {
    char line[1024];

    for(;;) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static int runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);
    if (result == INTERPRET_COMPILE_ERROR) {
        return 65;
    }
    if (result == INTERPRET_RUNTIME_ERROR) {
        return 70;
    }
    return 0;
}

int main(int argc, const char* argv[]) {
    initVM();

    int exitCode;

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        exitCode = runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        return 64;
    }

    freeVM();
    return exitCode;
}
