#include <stdio.h>
#include <string.h>
#include "algmalloc.h"

void printFreeList()
{
    struct blockHeader *head = getFreeList();
    if (head != NULL) {
        printf("======= Printing Free List =======\n");
        struct blockHeader *node = head;
        while (node != NULL) {
            printf("Free Block at %p is size %zu\n", node, node->size);
            node = node->next;
        }
    } else {
        printf("No blocks in free list.\n");
    }
}

void printPointerList(void **ptrArray, size_t length)
{
    if (length > 0) {
        printf("======= Printing Pointer List =======\n");
    }
    for (int i = 0; i < length; i++) {
        struct blockHeader *metaData = (struct blockHeader*)((char*)ptrArray[i] - sizeof(struct blockHeader));
        printf("ptr%d at %p and is size %zu\n", i, ptrArray[i], metaData->size);
    }
}

void freeAllPointers(void **ptrArray, size_t length)
{
    for (int i = 0; i < length; i++) {
        algfree(ptrArray[i]);
    }
}

int main()
{
    printf("Allocating chunk of size 16...\n");
    char *ptr1 = algmalloc(16);
    printFreeList();

    printf("Allocating chunk of size 32...\n");
    char *ptr2 = algmalloc(32);
    printf("ptr2 at %p\n", ptr2);
    printFreeList(); // Bug here...why does a block of size 32 end up on the free list???

    printf("Allocating chunk of size 8...\n");
    char *ptr3 = algmalloc(8);
    printf("ptr3 at %p\n", ptr3);
    printFreeList();
    return 0;
}