#include <unistd.h>

struct blockHeader 
{
    size_t size;
    struct blockHeader *next;
    struct blockHeader *prev;
};
struct blockHeader *getFreeList();
void *algmalloc(size_t size);
void algfree(void *ptr);
