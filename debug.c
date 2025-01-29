#include <stdio.h>

#include "debug.h"
#include "value.h"

int readLongConst(Chunk* chunk, int offset);

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    // return the offset of the next instruction
    // instruction size: 1 byte 
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    // return the offset of the next instruction
    // instruction size: 2 byte 
    return offset + 2;
}

static int constantLongInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = readLongConst(chunk, offset);
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    // return the offset of the next instruction
    // instruction size: 4 byte (1 byte instruction, 3 bytes constant direction)
    return offset + 4;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    if (offset > 0
        &&
        //chunk->lines[offset] == chunk->lines[offset - 1]
        getLine(chunk, offset) == getLine(chunk, offset - 1)) {
        printf("   | ");
    } else {
        //printf("%04d ", chunk->lines[offset]);
        printf("%04d ", getLine(chunk, offset));
    }

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_NEGATE: 
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        default:
            printf("Unknown code %d\n", instruction);
            return offset + 1;
    }
}

int readLongConst(Chunk* chunk, int offset) {
    int first = chunk->code[offset + 1];
    int second = chunk->code[offset + 2];
    int third = chunk->code[offset + 3];
    return first + UINT8_MAX * second * third;
}







