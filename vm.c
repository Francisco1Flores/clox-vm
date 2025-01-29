#include "common.h"
#include "debug.h"
#include "vm.h"
#include "stdio.h"
#include "compiler.h"


VM vm;

int readLongConstBytes(); 

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM(){
    resetStack();
}

void freeVM(){
}

void push(Value value){
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

Value peek() {
    return *(vm.stackTop - 1);
}

Value* peekDir() {
    return vm.stackTop - 1;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG() (vm.chunk->constants.values[readLongConstBytes()])
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        *peekDir() = peek() op b; \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("        ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                push(constant);
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_NEGATE: {
                *peekDir() = -peek();
                //push(-pop());
                break;
            } 
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
            case OP_ADD: {
                BINARY_OP(+);
                break;
            }
            case OP_SUBTRACT: {
                BINARY_OP(-);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(*);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(/);
                break;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OP
}

InterpretResult interpret(char* source) {
    compile(source);
    return INTERPRET_OK;
    //vm.chunk = chunk;
    //vm.ip = vm.chunk->code;
    //return run();
}

int readLongConstBytes() {
   int first = *vm.ip++;
   int second = *vm.ip++;
   int third = *vm.ip++;
   return first + UINT8_MAX * second * third;
}








