/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <memory/mem.h>
#include <memory/memdefs.h>
#include <memory/page.h>
#include <memory/pmm.h>
#include <memory/vmm.h>

#include <hw/serial.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>

#define PAGE_SIZE    4096
#define VIRTUAL_BASE 0xC0000000

#define VADDR_GET_PAGE_DIR_IDX(vaddr) (((vaddr) >> 22) & 0x3FF)
#define VADDR_GET_PAGE_TAB_IDX(vaddr) (((vaddr) >> 12) & 0x3FF)
#define VADDR_GET_PAGE_FRM_OFF(vaddr) ((vaddr)&0xFFF)

extern uint8_t __start;
extern uint8_t __end;

extern uint8_t SYS_PGDIR;

extern uint32_t __vmm_stack_base;
extern uint32_t __vmm_stack_top;

static PageDirectoryEntry *g_pgdir;

static uint32_t *g_stack_base;
static uint32_t *g_stack_top;

static uint32_t g_stack_idx;

static uint32_t g_stack_size;

static void push_address(VirtualAddress virt);

static VirtualAddress pop_address(void);

void vmm_init()
{
    g_pgdir = (PageDirectoryEntry *)(&SYS_PGDIR);
    KSLOG("initial page directory located at %p\n", (void *)(g_pgdir));

    // initialize temporary free address stack
    g_stack_base = &__vmm_stack_base;
    g_stack_top  = &__vmm_stack_top;
    g_stack_size = (g_stack_top - g_stack_base) / sizeof g_stack_base;
    g_stack_idx  = 0;

    // ONLY TEMPORARY
    for (uint32_t i = 0; i < g_stack_size; i++)
        push_address(0xA0000000 + i * PAGE_SIZE);

    page_load_page_directory((PageDirectoryEntry *)((VirtualAddress)(g_pgdir)-VIRTUAL_BASE));
}

void *vmm_allocate_page()
{
    void          *page = pmm_allocate_page();
    VirtualAddress virt = pop_address();

    vmm_map_page((PhysicalAddress)(page), virt);

    KSLOG("returning page 0x%08x\n", virt);

    return (void *)(virt);
}

void vmm_free_page(void *page)
{
    if (((VirtualAddress)(page)&0xFFF))
    {
        KSLOG("error: page %p not page-aligned\n", page);
        return;
    }

    PhysicalAddress phys = vmm_unmap_page((VirtualAddress)(page));

    pmm_free_page((void *)(phys));
    push_address((VirtualAddress)(page));
}

void vmm_map_page(PhysicalAddress phys, VirtualAddress virt)
{
    KSLOG("mapping page 0x%08X to physical address 0x%08X\n", virt, phys);
    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    if (!SETBIT(g_pgdir[dir_idx].access_byte, PGD_AX_PRESENT))
    {
        void *page_tab   = pmm_allocate_page();
        g_pgdir[dir_idx] = page_create_page_directory_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL,
                                                            (PhysicalAddress)(page_tab));
    }
    PageTableEntry *page_tab = (PageTableEntry *)(0xFFC00000 + dir_idx * 0x1000);
    page_tab[tab_idx] = page_create_page_table_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL, phys & 0xFFFFF000);
}

PhysicalAddress vmm_unmap_page(VirtualAddress virt)
{
    KSLOG("unmapping page 0x%08X\n", virt);
    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    PageTableEntry *page_tab = (PageTableEntry *)(0xFFC00000 + dir_idx * 0x1000);
    PhysicalAddress phys     = page_tab[tab_idx].address_upper_20 << 12;
    page_tab[tab_idx]        = page_create_page_table_entry(0, 0);

    page_flush_tlb();

    return phys;
}

void push_address(VirtualAddress virt)
{
    KSLOG("pushing address 0x%08X to the VMM stack\n", virt);

    if (g_stack_idx >= g_stack_size)
    {
        KSLOG("error: free address stack full\n");
        return;
    }

    g_stack_base[g_stack_idx] = virt & 0xFFFFF000;
    ++g_stack_idx;
}

VirtualAddress pop_address()
{
    KSLOG("popping last free address from VMM stack\n");

    if (g_stack_idx < 0)
    {
        KSLOG("error: free address stack empty\n");
        return 0xDEADBEEF; // not page-aligned
    }

    uint32_t pop = g_stack_base[g_stack_idx];
    if (g_stack_idx > 0)
        --g_stack_idx;

    return pop;
}
