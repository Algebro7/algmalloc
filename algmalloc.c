#include "algmalloc.h"

#define PAGE_SIZE 4096

int headerSize = sizeof(struct blockHeader);

struct blockHeader *freeList = NULL;

struct blockHeader *getFreeList()
{
    return freeList;
}

void addNodeToFreeList(struct blockHeader *node)
{
    struct blockHeader *curNode;
    if (freeList == NULL) {
        freeList = node;
        node->prev = NULL;
        node->next = NULL;
    } else {
        curNode = freeList;
        while (curNode->size < node->size && curNode->next != NULL) {
            curNode = curNode->next;
        }
        // Insert before current node
        if (curNode->size > node->size) {
            if (curNode->prev != NULL) {
                curNode->prev->next = node;
                node->prev = curNode->prev;
            } else {
                node->prev = NULL;
            }
            curNode->prev = node;
            node->next = curNode;
        // Insert after current node (we've reached the end of the list)
        } else {
            curNode->next = node;
            node->prev = curNode;
            node->next = NULL;
        }
    }   
}

struct blockHeader *searchFreeList(size_t size)
{
    struct blockHeader *node = freeList;

    while (node != NULL) {
        if (node->size >= size) {
            break;
        }
        node = node->next;
    }

    return node;
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

void removeNodeFromFreeList(struct blockHeader *block)
{

    if (block->prev != NULL) {
        if (block->next != NULL) {
            // Block is in the middle
            block->prev->next = block->next;
            block->next->prev = block->prev;
        } else {
            // Block is the tail
            block->prev->next = NULL;
        }
    } else {
        // Block is the head
        if (block->next != NULL) {
            block->next->prev = NULL;
            freeList = block->next;
        } else {
            // Free List only contains this block
            freeList = NULL;
        }
    }

    block->prev = NULL;
    block->next = NULL;
}

// Takes an allocated block, resizes it, and returns it. The leftovers are
// made into a new block and added to the free list
struct blockHeader *shrinkBlock(struct blockHeader *block, size_t size)
{
    struct blockHeader *newNode, *curNode;

    // if (block->next != NULL || block->prev != NULL) {
    //     removeNodeFromFreeList(block);
    // }
    curNode = freeList;
    while (curNode != NULL && curNode != block) {
        curNode = curNode->next;
    }
    if (block == curNode) {
        removeNodeFromFreeList(block);
    }

    newNode = (struct blockHeader*)((char*)block + headerSize + size);
    newNode->size = block->size - headerSize - size;
    block->size = size;

    addNodeToFreeList(newNode);
    return block;
}

void *allocateNewBlock(size_t size)
{
    size_t breakSize, totalSize;
    void *ptr = NULL;
    struct blockHeader *newBlock;

    totalSize = size + headerSize;

    breakSize = findBreakSize(totalSize);
    newBlock = incBreak(totalSize);     
    newBlock->size = breakSize - headerSize;
    newBlock->prev = NULL;
    newBlock->next = NULL;
    newBlock = shrinkBlock(newBlock, size);
    ptr = (char*)newBlock + headerSize;
    return ptr;
}

void *algmalloc(size_t size)
{
    void *ptr = NULL;
    struct blockHeader *node;

    // Round size to next multiple of 4
    size += 3; 
    size &=~3;
    
    // If there is no freeList, call sbrk and initialize one
    if (freeList == NULL) {
        ptr = allocateNewBlock(size);
    } else {
        node = searchFreeList(size);

        if (node != NULL) {
            node = shrinkBlock(node, size);
            ptr = (char*)node + headerSize;
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
    addNodeToFreeList(node);
}
