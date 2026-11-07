#include "arenaalloc.h"
#include <stdint.h>

arena_block_t *_get_arena_block(arena_t *arena, uint64_t size);
int _add_arena_block(arena_t *arena, uint64_t min_size);
static inline uint64_t max(uint64_t, uint64_t);

arena_t arena_init(uint64_t max_size) {
  arena_block_t *arena_block = ARENA_MALLOC(sizeof(arena_block_t));
  if (arena_block == NULL) {
    return (arena_t){
        .arena_block_start = NULL,
        .arena_block_end = NULL,
    };
  }
  *arena_block = (arena_block_t){
      .base = ARENA_MALLOC(max_size),
      .max_size = max_size,
      .allocated = 0,
      .next = NULL,
  };

  return (arena_t){
      .arena_block_start = arena_block,
      .arena_block_end = arena_block,
  };
}

void *arena_alloc(arena_t *arena, uint64_t size) {
  // get the first block with (size) number of bytes free
  arena_block_t *arena_block = _get_arena_block(arena, size);
  void *result = arena_block->base + arena_block->allocated;
  arena_block->allocated += size;
  return result;
}

void arena_free(arena_t *arena) {
  arena_block_t *arena_block_ptr = arena->arena_block_start;
  while (arena_block_ptr != NULL) {
    arena_block_t *next = arena_block_ptr->next;
    ARENA_FREE(arena_block_ptr->base);
    ARENA_FREE(arena_block_ptr);
    arena_block_ptr = next;
  }
}

arena_block_t *_get_arena_block(arena_t *arena, uint64_t size) {
  arena_block_t *arena_block = arena->arena_block_start;
  while (arena_block != NULL) {
    uint64_t remaining = arena_block->max_size - arena_block->allocated;
    if (remaining >= size) {
      return arena_block;
    }
    arena_block = arena_block->next;
  }

  if (_add_arena_block(arena, size) != 0) {
    return NULL;
  }
  return arena->arena_block_end;
}

int _add_arena_block(arena_t *arena, uint64_t min_size) {
  uint64_t max_size = max(arena->arena_block_start->max_size, min_size);
  arena_block_t *arena_block = ARENA_MALLOC(sizeof(arena_block_t));
  if (arena_block == NULL) {
    // malloc does not have enough memory
    return 1;
  }

  *arena_block = (arena_block_t){
      .base = ARENA_MALLOC(max_size),
      .max_size = max_size,
      .allocated = 0,
      .next = NULL,
  };

  arena->arena_block_end->next = arena_block;
  arena->arena_block_end = arena_block;
  return 0;
}

static inline uint64_t max(uint64_t a, uint64_t b) { return a > b ? a : b; }
