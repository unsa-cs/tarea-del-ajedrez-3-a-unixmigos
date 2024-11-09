#include "chess.h"
#include "figures.h"
#include "gc.h"

void display() {
  char *test[] = {"____", "____", "____", "____", 0};
  char **blackSquare = reverse(test);
  interpreter(blackSquare);
  garbageCollector();
}
