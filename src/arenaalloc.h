// This is an stb style header file.
// Just define ARENA_ALLOC_IMPLEMENTATION once for the implementation code.
#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stdint.h>
#include <stdio.h>

#ifndef ARENA_MAX_SIZE
#define ARENA_MAX_SIZE 1024
#endif

// you can now define your own malloc and free to be used instead
// by defining ARENA_MALLOC and ARENA_FREE
#if !defined(ARENA_FREE) && !defined(ARENA_MALLOC)
#include <stdlib.h>
#define ARENA_MALLOC(size) malloc((size))
#define ARENA_FREE(ptr) free((ptr))
#elif !defined(ARENA_FREE) || !defined(ARENA_MALLOC)
#error "ARENA_MALLOC & ARENA_FREE: neither or both should be defined"
#endif

#ifndef NULL
#define NULL 0
#endif

// Use arena_init to create a new arena
typedef struct arena_t {
  intptr_t base;
  uint64_t max_size;
  uint64_t allocated;

  struct arena_t *next;
} arena_t;

// Allocate the arena (type: arena_t).
// The arena will have its maximum size as passed in.
// If more data than was allocated (or is left of what was allocated)
// is requested, new alloc tables will be created.
// If max_size in 0, takes the default max size
arena_t *arena_init(uint64_t max_size);

// alloc an element from the arena.
// If the requested size is greater than the maximum allocation size,
// a new allocation table with size is created.
void *arena_alloc(arena_t *arena, uint64_t size);

// resets the arena, clearing its content but still rendering it usable.
void arena_reset(arena_t *arena);

// free the arena.
// DO NOT call free(arena), as that is also done here
void arena_free(arena_t *arena);

#endif // ARENA_ALLOC_H

#define ARENA_ALLOC_IMPLEMENTATION

// #define ARENA_ALLOC_IMPLEMENTATION once to include all of this
#ifdef ARENA_ALLOC_IMPLEMENTATION

#include <errno.h>

static inline uint64_t _max(uint64_t, uint64_t);

arena_t *arena_init(uint64_t max_size) {
  arena_t *arena = (arena_t *)ARENA_MALLOC(sizeof(arena_t));
  if (max_size == 0) {
    max_size = ARENA_MAX_SIZE;
  }
  arena->base = (intptr_t)ARENA_MALLOC(max_size);
  arena->allocated = 0;
  arena->max_size = max_size;
  arena->next = NULL;

  return arena;
}

void *arena_alloc(arena_t *arena, uint64_t size) {
  if (arena == NULL) {
    errno = EINVAL;
    return NULL;
  }

  arena_t *previous_arena_block = NULL;
  arena_t *current_arena_block = arena;

  // loop until a free block is found or current arena block is null
  while (current_arena_block != NULL) {
    if (current_arena_block->allocated + size <=
        current_arena_block->max_size) {
      void *current_memory =
          (void *)(current_arena_block->base + current_arena_block->allocated);
      current_arena_block->allocated += size;
      return current_memory;
    }
    previous_arena_block = current_arena_block;
    current_arena_block = current_arena_block->next;
  }

  // previous_arena_block now has the last element
  previous_arena_block->next = arena_init(_max(arena->max_size, size));

  current_arena_block = previous_arena_block->next;
  void *current_memory = (void *)current_arena_block->base;
  current_arena_block->allocated = size;

  return current_memory;
}

void arena_reset(arena_t *arena) {
  while (arena != NULL) {
    arena->allocated = 0;
    arena = arena->next;
  }
}

void arena_free(arena_t *arena) {
  while (arena != NULL) {
    arena_t *next = arena->next;
    ARENA_FREE((void *)arena->base);
    ARENA_FREE(arena);
    arena = next;
  }
}

static inline uint64_t _max(uint64_t a, uint64_t b) { return a < b ? b : a; }

#endif // ARENA_ALLOC_IMPLEMENTATION
