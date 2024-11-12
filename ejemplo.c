#include "chess.h"
#include "figures.h"
#include "gc.h"

void display() {
  char **blackSquare = join(reverse(whiteSquare), whiteSquare);
  interpreter(blackSquare);
  garbageCollector();
}
