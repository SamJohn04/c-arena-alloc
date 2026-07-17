# Arena Allocator

Allocate all you want, free just once.

## Usage

This is an `stb`-style implementation.
Copy `arenaalloc.h` into your projects.
Define `ARENA_ALLOC_IMPLEMENTATION` once.

Example given:
- main.c
```
#include <stdio.h>
#include "arenaalloc.h"

struct node_t {
    int value;
    struct node_t* next;
};

int main() {
    // initialize the allocator with the maximum expected size
    // say, 128 bytes
    arena_t* arena = new_arena(128);

    // allocate to a struct
    struct node_t* node = alloc_from_arena(arena, sizeof(struct node_t));
    printf("Successful!\n");

    // ...

    // free it all
    free_arena(arena);
}
```
