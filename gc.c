#include <stdlib.h>
#include "gc.h"

#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura Node para manejar la lista de punteros en cada entrada de memoria
typedef struct PointerNode{
  void** pointer;
  struct PointerNode* next;
} PointerNode;

// Definición de la estructura MemoryEntry para el diccionario de la memoria reservada
typedef struct MemoryEntry{
  void* memory;
  PointerNode* pointers;
  struct MemoryEntry* next;
} MemoryEntry;

MemoryEntry* memoryList = NULL; // Lista enlazada para las entradas de memoria

// Funcion para mostrar el diccionario
void showDictionary(){
  MemoryEntry* current = memoryList;
  fprintf(stderr, "--------------DICTIONARY--------------\n");
  while(current){
    fprintf(stderr, "[CLAVE]: MEMORY %p\n", current->memory);
    PointerNode* ptr = current->pointers;
    fprintf(stderr, "[VALOR(ES)]:\n");
    int contador = 0;
    while(ptr){
      fprintf(stderr, "Direccion de memoria de un puntero: %p\n", ptr->pointer);
      contador++;
      ptr = ptr->next;
    }
    fprintf(stderr, "Numero de punteros activos: %d\n", contador);
    current = current->next;
    fprintf(stderr, "######################################\n");
  }
  fprintf(stderr, "-------------END DICTIONARY----------\n");
}

// Función de ayuda para crear un nuevo nodo de puntero
PointerNode* createPointerNode(void** pointer){
  PointerNode* node = (PointerNode*)malloc(sizeof(PointerNode));
  node->pointer = pointer;
  node->next = NULL;
  return node;
}

// Función de ayuda para crear una nueva entrada de memoria
MemoryEntry* createMemoryEntry(void* memory){
  MemoryEntry* entry = (MemoryEntry*)malloc(sizeof(MemoryEntry));
  entry->memory = memory;
  entry->pointers = NULL;
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
  fprintf(stderr, "Direccion de memoria del puntero: %p\n", &(*pointer));
  MemoryEntry* entry = createMemoryEntry(*pointer);
  entry->pointers = createPointerNode(pointer);
  entry->next = memoryList;
  memoryList = entry;
  showDictionary();
}

// Función para agregar un puntero adicional que apunte a la misma memoria
void addPointer(void** new_pointer, void* existing_memory){
  MemoryEntry* current = memoryList;
  while(current){
    if(current->memory == existing_memory){
      PointerNode* newNode = createPointerNode(new_pointer);
      newNode->next = current->pointers;
      current->pointers = newNode;
      return;
    }
    current = current->next;
  }
  fprintf(stderr, "Error: la memoria especificada no está registrada\n");
}

// Función para desvincular un puntero de la entrada de memoria correspondiente
void unregisterPointer(void** pointer){
  fprintf(stderr, "puntero a desvincular %p\n", pointer);
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
        fprintf(stderr, "------------DESVINCULADO---------\n");
        showDictionary();
        return;
      }
      prev = ptr;
      ptr = ptr->next;
    }
    current = current->next;
  }
  fprintf(stderr, "---------------NO DESVINCULADO---------\n");
  showDictionary();
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

