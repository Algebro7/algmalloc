#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE 4096

struct blockHeader
{
    size_t size;
    struct blockHeader *next;
    struct blockHeader *prev;
};

int headerSize = sizeof(struct blockHeader);

struct blockHeader *freeList = NULL;

struct blockHeader *getFreeList()
{
    return freeList;
}

size_t findBreakSize(size_t size)
{
    int pages;
    
    pages = 4;
    while (PAGE_SIZE * pages <= size)
        pages += 4;
    
    return PAGE_SIZE * pages;
}

void *incBreak(size_t size)
{
    size_t breakSize; 
    void *ptr = NULL;

    breakSize = findBreakSize(size);
    ptr = sbrk(breakSize);

    return ptr;
}

struct blockHeader *shrinkBlock(struct blockHeader *blk, size_t totalSize)
{
    size_t prevSize;
    struct blockHeader *prevNode, *nextNode;

    prevSize = blk->size;
    prevNode = blk->prev;
    nextNode = blk->next;

    blk->size = totalSize - headerSize;

    blk = (struct blockHeader*)((char*)blk + totalSize);
    blk->size = prevSize - totalSize;

    if (prevNode != NULL)
        prevNode->next = blk;
    else
        freeList = blk;

    blk->prev = prevNode;
    blk->next = nextNode;

    return blk;
}

void appendNodeToFreeList(struct blockHeader *node)
{
    struct blockHeader *freeBlock; 

    if (freeList == NULL) {
        node->prev = NULL;
        freeList = node;
    } else {
        freeBlock = freeList;
        // Find last node in the free list
        while (freeBlock->next != NULL)
            freeBlock = freeBlock->next;

        freeBlock->next = node;
        node->prev = freeBlock;
    }
        
    node->next = NULL;
}

void prependNodeToFreeList(struct blockHeader *node)
{
    if (freeList != NULL)
        freeList->prev = node;
        
    node->prev = NULL;
    node->next = freeList;
    freeList = node;
}

struct blockHeader *searchFreeList(size_t size)
{
    struct blockHeader *node = freeList;

    while (node != NULL) {
        if (node->size >= size)
            break;
        node = node->next;
    }

    return node;
}

void *allocateNewBlock(size_t size)
{
    size_t breakSize, totalSize;
    void *ptr = NULL;
    void *newBreak = NULL;
    struct blockHeader *newBlock, *node;

    totalSize = size + headerSize;

    breakSize = findBreakSize(totalSize);
    newBreak = incBreak(totalSize);
    newBlock = newBreak;        
    newBlock->size = breakSize - headerSize;
    ptr = (char*)newBlock + headerSize;
    
    node = shrinkBlock(newBlock, totalSize);
    
    appendNodeToFreeList(node);

    return ptr;
}

void *algmalloc(size_t size)
{
    size_t totalSize;
    void *ptr = NULL;
    struct blockHeader *node;

    // Round size to next multiple of 4
    size += 3; 
    size &=~3;

    totalSize = headerSize + size;
    
    // If there is no freeList, call sbrk and initialize one
    if (freeList == NULL) {
        ptr = allocateNewBlock(size);
    } else {
        node = searchFreeList(totalSize);

        if (node != NULL) {
            ptr = (char*)node + headerSize;
            node = shrinkBlock(node, totalSize);
        } else {
            ptr = allocateNewBlock(size);
        }
    }

    return ptr;
}

void algfree(void *ptr)
{
    struct blockHeader *node;

    node = ptr - headerSize;
    prependNodeToFreeList(node);
}
