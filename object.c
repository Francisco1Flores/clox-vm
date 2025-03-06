#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) (type*)allocateObject(sizeof(type), objectType)
#define ALLOCATE_OBJ_STRING(type, objectType, length) (type*)allocateObject(sizeof(type) + sizeof(char[length]), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;

    object->next = vm.objects;
    vm.objects = object;

    return object;
}

static ObjString* allocateString(const char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ_STRING(ObjString, OBJ_STRING, length + 1);
    string->length = length;
    string->hash = hash;
    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';
    tableSet(&vm.strings, string, NIL_VAL);
    //FREE_ARRAY(char, chars, length);
    return string;
}

uint32_t hashString(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t) key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString* takeString(char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    ObjString* string = interned != NULL ? interned : allocateString(chars, length, hash);
    FREE_ARRAY(char, chars, length);
    return string;
}

ObjString* copyString(const char* chars, int length) {
    //char* heapChars = ALLOCATE(char, length + 1);
    //memcpy(heapChars, chars, length);
    //heapChars[length] = '\n';
    uint32_t hash = hashString(chars, length);

    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        return interned;
    }

    return allocateString(chars, length, hash);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}
