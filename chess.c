#include <stdio.h>
#include <stdlib.h>
#include "chess.h"
#include "gc.h"

char*** head;

int countRow(char** piece){
  int count = 0;
  while(*piece){
    piece++;
    count++;
  }
  return count;
}

int countCol(char* piece){
  char* tmp = piece;
  int count = 0;
  while(*tmp){
    tmp++;
    count++;
  }
  return count;
}

char** allocateMemory(int rows, size_t cols){
  char** newFig;
  head = &newFig;
  //fprintf(stderr, "Direccion de memoria de newFig(allocateMemory): %p\n", &newFig);
  memoryAlloc((void**)&newFig, sizeof(char*)*(rows + 1));
  for(int i = 0; i < rows; i++)
    memoryAlloc((void**)&newFig[i], sizeof(char)*(cols + 1));
  return newFig;
}

void unlinkMemory(char** fig){
  countMemoryEntries();
  for(int i = 0; fig[i]; i++)
    unregisterPointer((void**)&fig[i]);
  countMemoryEntries();
  //fprintf(stderr, "Direccion de memoria de fig(unlinkMemory): %p\n", &fig);
  //unregisterPointer((void**)&fig);
  unregisterPointer((void**)head);
  countMemoryEntries();
}

char** reverse(char** fig){
  int rows = 0;
  while(fig[++rows]);
  int cols = 0;
  while(fig[0][++cols]);
  char** newFig = allocateMemory(rows, cols);
  for(int i = 0; fig[i]; i++){
    for(int j = 0; fig[0][j]; j++){
      if (fig[i][j] == '.')
        newFig[i][j] = '@';
      else if (fig[i][j] == '@')
        newFig[i][j] = '.';
      else if (fig[i][j] == '_')
        newFig[i][j] = '=';
      else if (fig[i][j] == '=')
        newFig[i][j] = '_';
      else
        newFig[i][j] = fig[i][j];
    }
    newFig[i][cols] = 0;
  }
  newFig[rows] = 0;
  unlinkMemory(newFig);
  return newFig;
}

char** join(char** piece1, char** piece2){
  int row = countRow(piece1);
  int col1 = countCol(*piece1);
  int col2 = countCol(*piece2);
  int colTotal = col1 + col2;

  char** newPiece = allocateMemory(row, colTotal);

  for(int i = 0; i < row; i++){
    for(int j = 0; j < col1; j++){
      newPiece[i][j] = piece1[i][j];
    }
    for(int k = 0; k < col2; k++){
      newPiece[i][k + col1] = piece2[i][k];
    }
    newPiece[i][colTotal] = 0;
  }
  newPiece[row] = 0;
  unlinkMemory(newPiece);
  return newPiece;
}
