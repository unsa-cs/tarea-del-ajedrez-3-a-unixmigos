#include <stdlib.h>
#include "gc.h"
#include <stdio.h>

// Definición de la estructura MemoryEntry para el diccionario de la memoria reservada
typedef struct MemoryEntry{
  void* memory;
  int countRef;
  struct MemoryEntry* next;
} MemoryEntry;

MemoryEntry* memoryList = NULL; // Lista enlazada para las entradas de memoria

// Función de ayuda para crear una nueva entrada de memoria
MemoryEntry* createMemoryEntry(void* memory){
  MemoryEntry* entry = (MemoryEntry*)malloc(sizeof(MemoryEntry));
  entry->memory = memory;
  entry->countRef = 1;
  entry->next = NULL;
  return entry;
}

// Función para asignar memoria y registrar la entrada en el diccionario
void memoryAlloc(void** pointer, size_t size){
  *pointer = malloc(size);
  if(!(*pointer)){
    fprintf(stderr, "Error al asignar memoria\n");
    return;
  }
  //fprintf(stderr, "Direccion de memoria del puntero: %p\n", &(*pointer));
  MemoryEntry* entry = createMemoryEntry(*pointer);
  entry->next = memoryList;
  memoryList = entry;
  //showDictionary();
}

// Función para agregar un puntero adicional que apunte a la misma memoria
void addPointer(void** new_pointer, void* existing_memory){
  MemoryEntry* current = memoryList;
  while(current){
    if(current->memory == existing_memory){
      current->countRef++;
      return;
    }
    current = current->next;
  }
  fprintf(stderr, "Error: la memoria especificada no está registrada\n");
}

// Función para desvincular un puntero de la entrada de memoria correspondiente
void unregisterPointer(void** pointer){
  //fprintf(stderr, "puntero a desvincular %p\n", pointer);
  MemoryEntry* current = memoryList;
  while(current){
    PointerNode* prev = NULL;
    PointerNode* ptr = current->pointers;
    while(ptr){
      if(ptr->pointer == pointer){
        if(prev)
          prev->next = ptr->next;
        else
          current->pointers = ptr->next;
        free(ptr);
        //fprintf(stderr, "------------DESVINCULADO---------\n");
        //showDictionary();
        return;
      }
      prev = ptr;
      ptr = ptr->next;
    }
    current = current->next;
  }
  //fprintf(stderr, "---------------NO DESVINCULADO---------\n");
  //showDictionary();
}

// Función de recolección de basura que libera memoria sin referencias activas
void garbageCollector(){
  MemoryEntry* prevEntry = NULL;
  MemoryEntry* currentEntry = memoryList;
  while(currentEntry){
    if(currentEntry->pointers){
      prevEntry = currentEntry;
      currentEntry = currentEntry->next;
    }else{
      // Sin referencias activas, liberar memoria
      free(currentEntry->memory);
      if(prevEntry)
        prevEntry->next = currentEntry->next;
      else
        memoryList = currentEntry->next;
      MemoryEntry* toFree = currentEntry;
      currentEntry = currentEntry->next;
      free(toFree);
    }
  }
}

int countMemoryEntries(){
  int count = 0;
  MemoryEntry* current = memoryList;
  while(current){
    if(current->pointers){
      count++;
    }
    current = current->next;
  }
  fprintf(stderr, "[DEBUG] memory with references: %d\n", count);
  return count;
}

