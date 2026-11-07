#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// you can define your own malloc and free to be used instead
// by defining ARENA_MALLOC and ARENA_FREE
#if !defined(ARENA_FREE) && !defined(ARENA_MALLOC)
#define ARENA_MALLOC(size) malloc((size))
#define ARENA_FREE(ptr) free((ptr))
#elif !defined(ARENA_FREE) || !defined(ARENA_MALLOC)
#error "ARENA_MALLOC & ARENA_FREE: neither or both should be defined"
#endif

typedef struct arena_block_t {
  uint64_t max_size;
  uint64_t allocated;
  void *base;
  struct arena_block_t *next;
} arena_block_t;

// Use arena_init to create a new arena
typedef struct {
  arena_block_t *arena_block_start;
  arena_block_t *arena_block_end;
} arena_t;

// Allocate the arena (type: arena_t).
// The arena will have its maximum size as passed in.
// If more data than was allocated (or is left of what was allocated)
// is requested, new alloc tables will be created.
// If max_size in 0, takes the default max size
arena_t arena_init(uint64_t max_size);

// alloc an element from the arena.
// If the requested size is greater than the maximum allocation size,
// a new allocation table with size is created.
void *arena_alloc(arena_t *arena, uint64_t size);

// free the arena.
void arena_free(arena_t *arena);

#endif // ARENA_ALLOC_H
