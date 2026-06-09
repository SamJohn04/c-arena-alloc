#include "arenaalloc.h"
#include <stdio.h>

int main(void) {
    // define for 1 KB
    arena_t* arena = new_arena(1024);
    printf("Allocating an element of size %zu\n", sizeof(int));
    int* a = alloc_from_arena(arena, sizeof(int));
    *a = 10;
    printf("\tAllocation successful\n");
    printf("\t10 == %d\n", *a);
    printf("\tarena next is NULL: %s\n", arena->next == NULL ? "true" : "false");
    printf("\n");

    printf("Allocating an element of size 300\n");
    void* b = alloc_from_arena(arena, 300);
    if (b == NULL) {
        printf("\tAllocation failed\n");
        return 1;
    } else if (b == a) {
        printf("\tAllocation gave the same pointer twice\n");
        return 2;
    }
    printf("\tAllocation successful\n");
    printf("\n");

    printf("Allocating an element of size 2048\n");
    void* c = alloc_from_arena(arena, 2048);
    if (c == NULL) {
        printf("\t- Allocation failed\n");
        return 1;
    }
    if (arena->next == NULL || arena->next->next != NULL) {
        printf("\t- Unexpected size of allocation tables\n");
        return 2;
    }
    printf("\tAllocation successful\n");
    printf("\n");

    printf("Allocating an element of size 200\n");
    void* d = alloc_from_arena(arena, 200);
    if (d == NULL) {
        printf("\t- Allocation failed\n");
        return 1;
    }
    if (arena->next == NULL || arena->next->next != NULL) {
        printf("\t- Unexpected size of allocation tables\n");
        return 2;
    }
    printf("\tAllocation successful\n");
    printf("\n");

    printf("Free the arena\n");
    free_arena(arena);
    printf("\tFree successful\n");
}
