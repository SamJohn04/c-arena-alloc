#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stddef.h>
#include <stdint.h>

// the inner workings of an arena are subject to change
// please use the methods provided instead
typedef struct arena_t {
    void*           base;
    void*           current;
    uint64_t        max_size;
    uint64_t        remaining;

    struct arena_t* next;
} arena_t;

// Allocate the arena (type: arena_t)
// The arena will have its maximum size as passed in.
// In a perfect world, this will be the only syscall made.
// If more data than was allocated (or is left of what was allocated)
//  is requested, new alloc tables will be created.
// The algorithm checks each arena node till it finds an area big enough,
//  so its best to keep the arena node big enough
arena_t* new_arena(size_t max_size);

// alloc an element from the arena
// If the requested size is greater than the maximum allocation size,
//  a new allocation table with size is created.
void* alloc_from_arena(arena_t* arena, size_t size);

// free the arena.
// DO NOT call free(arena), as that is also done here
void free_arena(arena_t* arena);

#endif // ARENA_ALLOC_H
