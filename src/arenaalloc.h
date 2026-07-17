// This is an stb style header file.
// Just define ARENA_ALLOC_IMPLEMENTATION once for the implementation code.
#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stdlib.h>
#include <stdint.h>

// you can now define your own malloc to be used instead
// by defining ARENA_MALLOC
#ifndef ARENA_MALLOC
#define ARENA_MALLOC(size) malloc((size))
#endif // ARENA_MALLOC

// it is recommended for you to use the functions
// rather than directly access these values,
// as they are subject to change
typedef struct arena_t {
  void*           base;
  size_t          max_size;
  size_t          allocated;

  struct arena_t* next;
} arena_t;

// Allocate the arena (type: arena_t).
// The arena will have its maximum size as passed in.
// If more data than was allocated (or is left of what was allocated)
// is requested, new alloc tables will be created.
arena_t* new_arena(size_t max_size);

// alloc an element from the arena.
// If the requested size is greater than the maximum allocation size,
// a new allocation table with size is created.
void* alloc_from_arena(arena_t* arena, size_t size);

// resets the arena, clearing its content but still rendering it usable.
void reset_arena(arena_t* arena);

// free the arena.
// DO NOT call free(arena), as that is also done here
void free_arena(arena_t* arena);

// #define ARENA_ALLOC_IMPLEMENTATION once to include all of this
#ifdef ARENA_ALLOC_IMPLEMENTATION

#include <errno.h>

static inline size_t _max(size_t, size_t);

arena_t* new_arena(size_t max_size) {
  arena_t* arena = ARENA_MALLOC(sizeof(arena_t));
  arena->base      = ARENA_MALLOC(max_size);
  arena->allocated = 0;
  arena->max_size  = max_size;
  arena->next = NULL;

  return arena;
}

void* alloc_from_arena(arena_t* arena, size_t size) {
  if (arena == NULL) {
    errno = EINVAL;
    return NULL;
  }

  arena_t* previous_arena_block = NULL;
  arena_t* current_arena_block  = arena;

  // loop until a free block is found or current arena block is null
  while (current_arena_block != NULL) {
    if (current_arena_block->allocated + size <= current_arena_block->max_size) {
      void* current_memory            = (
          current_arena_block->base + current_arena_block->allocated);
      current_arena_block->allocated += size;
      return current_memory;
    }
    previous_arena_block = current_arena_block;
    current_arena_block  = current_arena_block->next;
  }

  // previous_arena_block now has the last element
  previous_arena_block->next = new_arena(_max(arena->max_size, size));

  current_arena_block            = previous_arena_block->next;
  void* current_memory           = current_arena_block->base;
  current_arena_block->allocated = size;

  return current_memory;
}

void reset_arena(arena_t* arena) {
  while (arena != NULL) {
    arena->allocated = 0;
    arena            = arena->next;
  }
}

void free_arena(arena_t* arena) {
  while (arena != NULL) {
    arena_t* next = arena->next;
    free(arena->base);
    free(arena);
    arena = next;
  }
}

size_t _max(size_t a, size_t b) {
  return a < b ? b : a;
}

#endif // ARENA_ALLOC_IMPLEMENTATION

#endif // ARENA_ALLOC_H
