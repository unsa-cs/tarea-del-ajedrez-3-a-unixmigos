#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <stdlib.h>

void memoryAlloc(void** pointer, size_t size);
void registerPointerToMemory(void** new_pointer, void* existing_memory);
void unregisterPointer(void** pointer);
void garbageCollector();
int countMemoryEntries();

#endif
