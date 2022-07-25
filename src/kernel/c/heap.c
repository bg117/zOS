#include <kernel/hw/serial.h>
#include <kernel/memory/heap.h>
#include <kernel/memory/memdefs.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log_macros.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <utils/mem.h>
#include <utils/type.h>

#define PAGE_SIZE 0x1000

typedef struct block_linked_list BlockLinkedList;

struct __attribute__((aligned(alignof(max_align_t)))) block_linked_list
{
    size_t           size;
    bool             free;
    BlockLinkedList *next;
};

static BlockLinkedList *g_heap;

static size_t g_total_pages;

static VirtualAddress g_virt_base;

extern uint8_t _eprog;

static void split_node(BlockLinkedList *node, size_t size);
static void merge_free_blocks(void);
static void log_heap(void)
{
    // log heap
    BlockLinkedList *hd = g_heap;
    KSLOG("heap:\n");
    while (hd)
    {
        serial_write_format_string("\tblock %p:\n\t\tsize: %u\n\t\tfree: %s\n", hd, hd->size, hd->free ? "yes" : "no");
        hd = hd->next;
    }
}

void heap_init(size_t init_size)
{
    MemoryAddress base = (MemoryAddress)&_eprog + 0xC0000000;

    g_total_pages = ALIGN(init_size, PAGE_SIZE) / PAGE_SIZE;
    g_heap        = (BlockLinkedList *)ALIGN(base, alignof(max_align_t));
    g_virt_base   = (VirtualAddress)g_heap;

    VirtualAddress virt_base = g_virt_base;

    for (size_t i = 0; i < g_total_pages; i++, virt_base += PAGE_SIZE)
    {
        // we unfortunately can't use vmm_allocate_page/s as we need more control
        vmm_map_page((PhysicalAddress)pmm_allocate_page(), virt_base);
    }

    g_heap->size = g_total_pages * PAGE_SIZE - sizeof(BlockLinkedList);
    g_heap->next = NULL;
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

    log_heap();

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
    else
    {
        KSLOG("allocation size too big, resizing heap\n");

        VirtualAddress virt_base     = g_virt_base + g_total_pages * PAGE_SIZE;
        size_t         additional_pg = ALIGN(size, PAGE_SIZE) / PAGE_SIZE;

        for (size_t i = 0; i < additional_pg; i++, virt_base += PAGE_SIZE)
            vmm_map_page((PhysicalAddress)pmm_allocate_page(), virt_base);

        g_total_pages += additional_pg;
        p->size += additional_pg * PAGE_SIZE;

        split_node(p, size);
    }

    MemoryAddress ret = (MemoryAddress)p + sizeof(BlockLinkedList);

    KSLOG("returning block at address 0x%08X\n", ret);
    return (void *)ret;
}

void *heap_reallocate(void *old_ptr, size_t new_size)
{
    KSLOG("reallocating block 0x%08X to new size of %u\n", (MemoryAddress)old_ptr, new_size);

    if (new_size == 0)
    {
        KSLOG("error: cannot reallocate block to size 0\n");
        return NULL; // original pointer is unmodified
    }

    void *new_blk = heap_allocate(new_size);
    mem_copy(new_blk, old_ptr, new_size);

    heap_free(old_ptr);
    return new_blk;
}

void heap_free(void *ptr)
{
    for (BlockLinkedList *p = g_heap; p->next != NULL; p = p->next)
    {
        if ((void *)((MemoryAddress)p + sizeof(BlockLinkedList)) == ptr)
        {
            KSLOG("found block at 0x%08X, freeing\n", (MemoryAddress)ptr);
            p->free = true;

            KSLOG("merging existing free blocks\n");
            merge_free_blocks();

            log_heap();

            return;
        }
    }

    KSLOG("error: cannot find block at 0x%08X\n", (MemoryAddress)ptr);
}

void split_node(BlockLinkedList *node, size_t size)
{
    static const size_t align        = alignof(max_align_t);
    size_t              aligned_size = ALIGN(size, align);

    MemoryAddress base = (MemoryAddress)node + aligned_size + sizeof(BlockLinkedList);
    base               = ALIGN(base, align);

    BlockLinkedList *new_node = (BlockLinkedList *)base;

    new_node->size = node->size - aligned_size - sizeof(BlockLinkedList);
    new_node->free = true;
    new_node->next = node->next;

    node->size = aligned_size;
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
