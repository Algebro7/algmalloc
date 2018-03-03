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
    printFreeList();
    void *ptrArray[] = { algmalloc(16), algmalloc(32), algmalloc(45056), algmalloc(2048) };
    printFreeList();
    printPointerList(ptrArray, sizeof(ptrArray) / sizeof(ptrArray[0]));
    
    memset(ptrArray[0], 'A', 16);
    memset(ptrArray[1], 'B', 32);
    memset(ptrArray[2], 'C', 45056);
    memset(ptrArray[3], 'D', 2048);

    algfree(ptrArray[1]);
    printFreeList();
    //ptrArray[1] = algmalloc(8);
    // ptrArray[1] = algmalloc(8);
    // ptrArray[1][6] = '\n';
    // ptrArray[1][7] = '\0';
    // printf("%s", ptr);
    // printFreeList();
    // printPointerList(ptrArray, sizeof(ptrArray) / sizeof(ptrArray[0]));

    // printFreeList();
    // //freeAllPointers(ptrArray, sizeof(ptrArray) / sizeof(ptrArray[0]));
    // //for (int i = 0; i < 4; i++)
    // //    algfree(ptrArray[i]);
    // getchar();

    // printFreeList();
    return 0;
}