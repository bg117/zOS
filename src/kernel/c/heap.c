#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log_macros.h>

#include <kernel/memory/memdefs.h>
#include <kernel/memory/vmm.h>

#include <kernel/hw/serial.h>

#include <utils/mem.h>
#include <utils/type.h>

#define PAGE_SIZE 0x1000

typedef struct block_linked_list BlockLinkedList;

struct block_linked_list
{
    size_t           size;
    uint16_t         magic;
    bool             free;
    BlockLinkedList *next;
};

static BlockLinkedList *g_heap;

static size_t g_total_pages;

static void split_node(BlockLinkedList *node, size_t size);
static void merge_free_blocks(void);
static void double_heap(size_t add_size);

void heap_init(size_t init_size)
{
    g_total_pages = ALIGN(init_size, PAGE_SIZE) / PAGE_SIZE;
    g_heap        = vmm_allocate_pages(g_total_pages);

    g_heap->magic = 0xBEAD; // idk
    g_heap->size  = g_total_pages * PAGE_SIZE - sizeof(BlockLinkedList);
    g_heap->next  = NULL;
}

void *heap_allocate(size_t size)
{
    KSLOG("allocating block of size %u\n", size);

    if (size == 0)
    {
        KSLOG("error: cannot allocate block of size 0\n");
        return NULL;
    }

    if (g_heap->size == 0)
    {
        KSLOG("error: heap empty\n");
        return NULL;
    }

    BlockLinkedList *p = g_heap;
    while (((p->size <= size) || !p->free) && p->next)
        p = p->next;

    if (p->size == size)
    {
        KSLOG("found block with exact size\n");
        p->free = false;
    }
    else if (p->size > size + sizeof(BlockLinkedList))
    {
        KSLOG("block too big, splitting node into two blocks\n");
        split_node(p, size);
    }
    else if (p->size < size)
    {
        KSLOG("allocation size too big, resizing heap\n");
        double_heap(size);
        return heap_allocate(size);
    }

    VirtualAddress ret = (VirtualAddress)(p + sizeof(BlockLinkedList));

    KSLOG("returning block at address 0x%08X\n", ret);
    return (void *)ret;
}

void heap_free(void *ptr)
{
    for (BlockLinkedList *p = g_heap; p->next != NULL; p = p->next)
    {
        if (p + sizeof(BlockLinkedList) == ptr)
        {
            KSLOG("found block at 0x%08X, freeing\n", (VirtualAddress)ptr);
            p->free = true;

            KSLOG("merging existing free blocks\n");
            merge_free_blocks();

            // log heap
            BlockLinkedList *hd = g_heap;
            KSLOG("heap:\n");
            while (hd)
            {
                serial_write_format_string(
                    "\tblock %p:\n\t\tsize: %u\n\t\tfree: %s\n", hd, hd->size, hd->free ? "yes" : "no");
                hd = hd->next;
            }
            return;
        }
    }

    KSLOG("error: cannot find block at 0x%08X\n", (VirtualAddress)ptr);
}

void split_node(BlockLinkedList *node, size_t size)
{
    BlockLinkedList *new_node = node + size + sizeof(BlockLinkedList);

    new_node->size  = node->size - size - sizeof(BlockLinkedList);
    new_node->free  = true;
    new_node->magic = 0xBEAD;
    new_node->next  = node->next;

    node->size = size;
    node->free = false;
    node->next = new_node;
}

void merge_free_blocks(void)
{
    BlockLinkedList *p = g_heap;

    while (p)
    {
        if (p->free && p->next && p->next->free)
        {
            p->size += p->next->size + sizeof(BlockLinkedList);
            p->next = p->next->next;
        }

        p = p->next;
    }
}

static void double_heap(size_t add_size)
{
    size_t new_size = g_total_pages * PAGE_SIZE + add_size;
    new_size        = ALIGN(new_size, PAGE_SIZE);

    void *new_heap = vmm_allocate_pages(new_size);
    mem_copy(new_heap, g_heap, g_total_pages * PAGE_SIZE);

    g_total_pages = new_size / PAGE_SIZE;
    g_heap        = new_heap;

    merge_free_blocks();
}
