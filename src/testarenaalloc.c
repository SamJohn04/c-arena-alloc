#define ARENA_ALLOC_IMPLEMENTATION
#include "arenaalloc.h"
#include <stdio.h>

void errorf(char *message) { fprintf(stderr, "[ERROR] %s\n", message); }

int main(void) {
  // define for 1 KB
  arena_t *arena = arena_init(1024);

  printf("Allocating an element of size %zu\n", sizeof(int));
  int *a = arena_alloc(arena, sizeof(int));
  if (a == NULL) {
    errorf("allocation failed");
    return 1;
  }
  printf("\tAllocation successful\n");
  printf("\n");

  printf("Allocating an element of size 300\n");
  void *b = arena_alloc(arena, 300);
  if (b == NULL) {
    errorf("allocation failed");
    return 1;
  } else if (b == a) {
    errorf("allocation gave the same pointer twice");
    return 1;
  }
  printf("\tAllocation successful\n");
  printf("\n");

  printf("Allocating an element of size 2048\n");
  void *c = arena_alloc(arena, 2048);
  if (c == NULL) {
    errorf("allocation failed");
    return 1;
  } else if (arena->next == NULL || arena->next->next != NULL) {
    errorf("unexpected size of allocation tables");
    return 1;
  }
  printf("\tAllocation successful\n");
  printf("\n");

  printf("Allocating an element of size 200\n");
  void *d = arena_alloc(arena, 200);
  if (d == NULL) {
    errorf("allocation failed");
    return 1;
  } else if (arena->next == NULL || arena->next->next != NULL) {
    errorf("unexpected size of allocation tables");
    return 1;
  }
  printf("\tAllocation successful\n");
  printf("\n");

  printf("Reset the arena\n");
  arena_reset(arena);
  void *e = arena_alloc(arena, 1);
  if (e != a) {
    printf("The first element of both goes is not the same");
    return 1;
  }
  printf("\tReset succesful\n");
  printf("\n");

  printf("Free the arena\n");
  arena_free(arena);
  printf("\tFree successful\n");
  printf("\n");
}
