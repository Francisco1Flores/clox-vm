#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "stdio.h"


void saveLine(Chunk* chunk, int line, int byteIndex);

void initChunk(Chunk* chunk){
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->nLines = 0;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, int byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        //chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    saveLine(chunk, line, chunk->count);
    //chunk->lines[chunk->count] = line;
    chunk->count++;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->nLines * 3);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value constant) {
    writeValueArray(&chunk->constants, constant);
    return chunk->constants.count - 1;
}

void saveLine(Chunk* chunk, int line, int byteIndex) {
    int i;
    for (i = 0; i < chunk->nLines * 3; i = i + 3) {
        if(chunk->lines[i] == line) {
            if (chunk->lines[i + 1] == -1) {
                chunk->lines[i + 1] = byteIndex;
                chunk->lines[i + 2] = byteIndex;
            } else {
                chunk->lines[i + 2] = byteIndex;
            }
            return;
        }
    }
    int oldCapacity = i;
    chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, oldCapacity + 3);
    chunk->lines[i] = line;

    chunk->lines[i + 1] = byteIndex;
    chunk->lines[i + 2] = byteIndex;

    chunk->nLines++;
}

int getLine(Chunk* chunk, int byteIndex) {
    for (int i = 0; i < chunk->nLines * 3; i = i + 3) {
        if (byteIndex >= chunk->lines[i+1] && byteIndex <= chunk->lines[i+2]) {
            return chunk->lines[i];
        }
    }
    return -1;
}
