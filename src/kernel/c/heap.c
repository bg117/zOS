#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel/misc/bit_macros.h"

#define PAGE_SIZE 0x1000

typedef struct block_linked_list BlockLinkedList;

struct block_linked_list
{
    size_t           size;
    bool             free;
    void            *address;
    BlockLinkedList *next;
};

static BlockLinkedList g_heap_head;

static size_t g_total_pages;

void heap_init(size_t init_size)
{
    g_total_pages = ALIGN(init_size, PAGE_SIZE);
}
