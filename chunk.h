#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"


typedef enum {
    OP_CONSTANT,        // 1 byte operand
    OP_CONSTANT_LONG,   // 3 bytes operand
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,     // 1 byte operand (index of constant value) 
    OP_SET_GLOBAL,     // 1 byte operand (index of constant value)
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    int nLines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, int byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value constant);
int addConstantLong(Chunk* chunk, Value constant);
int getLine(Chunk* chunk, int byteIndex);

#endif
