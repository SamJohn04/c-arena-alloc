#include "arenaalloc.h"
#include <stdio.h>

void errorf(char* message) {
    fprintf(stderr, "[ERROR] %s\n", message);
}

int main(void) {
    // define for 1 KB
    arena_t* arena = new_arena(1024);

    printf("Allocating an element of size %zu\n", sizeof(int));
    int* a = alloc_from_arena(arena, sizeof(int));
    if (a == NULL) {
        errorf("allocation failed");
        return 1;
    }
    printf("\tAllocation successful\n");
    if (arena->next != NULL) {
        errorf("arena next is not NULL");
        return 1;
    }
    printf("\tarena next is NULL\n");
    printf("\n");

    printf("Allocating an element of size 300\n");
    void* b = alloc_from_arena(arena, 300);
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
    void* c = alloc_from_arena(arena, 2048);
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
    void* d = alloc_from_arena(arena, 200);
    if (d == NULL) {
        errorf("allocation failed");
        return 1;
    } else if (arena->next == NULL || arena->next->next != NULL) {
        errorf("unexpected size of allocation tables");
        return 1;
    }
    printf("\tAllocation successful\n");
    printf("\n");

    printf("Free the arena\n");
    free_arena(arena);
    printf("\tFree successful\n");
}
