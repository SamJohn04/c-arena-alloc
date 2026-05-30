#include "arenaalloc.h"

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

size_t max(size_t, size_t);

arena_t* new_arena(size_t max_size) {
    arena_t* arena = malloc(sizeof(arena_t));

    arena->base      = malloc(max_size);
    arena->current   = arena->base;
    arena->remaining = max_size;

    arena->next = NULL;

    return arena;
}

void* alloc_from_arena(arena_t* arena, size_t size) {
    if (arena == NULL) {
        errno = EINVAL;
        return NULL;
    }
    arena_t* previous_arena_block = NULL;
    arena_t* current_arena_block = arena;
    while (current_arena_block != NULL) {
        if (current_arena_block->remaining <= size) {
            void* current = arena->current;
            arena->current += size;
            arena->remaining -= size;

            return current;
        }
        previous_arena_block = current_arena_block;
        current_arena_block = current_arena_block->next;
    }

    // previous_arena_block now has the last element
    previous_arena_block->next = new_arena(
            max(arena->max_size, size));

    void* current = arena->current;
    arena->current += size;
    arena->remaining -= size;

    return current;
}

void free_arena(arena_t* arena) {
    while (arena != NULL) {
        arena_t* next = arena->next;
        free(arena->base);
        free(arena);
        arena = next;
    }
}

size_t max(size_t a, size_t b) {
    if (a < b) {
        return b;
    }
    return a;
}
