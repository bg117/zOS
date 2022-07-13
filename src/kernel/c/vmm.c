/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/memory/mem.h>
#include <kernel/memory/memdefs.h>
#include <kernel/memory/page.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>

#include <kernel/hw/serial.h>

#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log_macros.h>

#define PAGE_SIZE    4096
#define VIRTUAL_BASE 0xC0000000
#define MAGIC_NUMBER 0xDEADC0DE
#define BITMAP_BITS  786432 // up to 0xC0000000 only

#define VADDR_GET_PAGE_DIR_IDX(vaddr) (((vaddr) >> 22) & 0x3FF)
#define VADDR_GET_PAGE_TAB_IDX(vaddr) (((vaddr) >> 12) & 0x3FF)
#define VADDR_GET_PAGE_FRM_OFF(vaddr) ((vaddr)&0xFFF)

/* defined in the linker script */
extern uint8_t _sprog, _svmm_bitmap, _eprog;

static uint32_t *g_bitmap;

// size of the bitmap in bits
static PhysicalAddress g_base;

extern uint32_t SYS_PGDIR;

static PageDirectoryEntry *g_pgdir;

static uint32_t get_first_free_idx(uint32_t *bit);

void vmm_init(void)
{
    g_pgdir = (PageDirectoryEntry *)(&SYS_PGDIR);
    KSLOG("initial page directory located at %p\n", (void *)g_pgdir);

    g_bitmap = (uint32_t *)(&_svmm_bitmap);

    g_base = 0;

    KSLOG("marking kernel as used\n");

    // mark bitmap and kernel as used
    uint32_t reserved = (PhysicalAddress)(&_eprog) - (PhysicalAddress)(&_sprog);
    reserved          = ALIGN(reserved, PAGE_SIZE) / PAGE_SIZE;
    for (uint32_t i = ((PhysicalAddress)(&_sprog) + VIRTUAL_BASE) / PAGE_SIZE, j = 0; j < reserved; i++, j++)
    {
        uint32_t idx = i / 64;
        uint32_t bit = i % 64;

        SETBITVAR(g_bitmap[idx], 1 << bit);
    }

    page_load_page_directory((PageDirectoryEntry *)((VirtualAddress)g_pgdir - VIRTUAL_BASE));
}

void *vmm_allocate_page()
{
    void    *page = pmm_allocate_page();
    uint32_t bit;
    uint32_t idx = get_first_free_idx(&bit);

    if ((PhysicalAddress)page == MAGIC_NUMBER)
    {
        KSLOG("error: no more physical pages left\n");
        return (void *)MAGIC_NUMBER;
    }

    if (idx == UINT32_MAX)
    {
        KSLOG("error: out of memory\n");
        return (void *)MAGIC_NUMBER;
    }

    SETBITVAR(g_bitmap[idx], 1 << bit);

    VirtualAddress addr = (idx * 64 + bit) * PAGE_SIZE;
    vmm_map_page((PhysicalAddress)page, addr);

    KSLOG("returning page 0x%x\n", addr);

    return (void *)addr;
}

void vmm_free_page(void *page)
{
    if (((VirtualAddress)page & 0xFFF))
    {
        KSLOG("error: page %p not page-aligned\n", page);
        return;
    }

    PhysicalAddress phys = vmm_get_phys((VirtualAddress)page);
    vmm_unmap_page((VirtualAddress)page);

    pmm_free_page((void *)phys);

    PhysicalAddress base_addr = g_base;
    for (uint32_t i = 0; i < BITMAP_BITS; i++, base_addr += PAGE_SIZE)
    {
        uint32_t idx = i / 64;
        uint32_t bit = i % 64;

        if (base_addr == (PhysicalAddress)page)
        {
            if (TESTBIT(g_bitmap[idx], (uint32_t)(1 << bit)))
                UNSETBITVAR(g_bitmap[idx], 1 << bit);
            else
                KSLOG("warning: page %p already free\n", page);

            return;
        }
    }

    KSLOG("error: page %p cannot be found in the bitmap\n", page);
}

void vmm_map_page(PhysicalAddress phys, VirtualAddress virt)
{
    KSLOG("mapping page 0x%X to physical address 0x%X\n", virt, phys);

    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    if (!TESTBIT(g_pgdir[dir_idx].access_byte, PGD_AX_PRESENT))
    {
        KSLOG("page table not present, creating one\n");

        void *page_tab   = pmm_allocate_page();
        g_pgdir[dir_idx] = page_create_page_directory_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL,
                                                            (PhysicalAddress)page_tab);
    }

    PageTableEntry *page_tab = (PageTableEntry *)(0xFFC00000 + dir_idx * PAGE_SIZE);
    page_tab[tab_idx] = page_create_page_table_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL, phys & 0xFFFFF000);
}

void vmm_unmap_page(VirtualAddress virt)
{
    KSLOG("unmapping page 0x%X\n", virt);
    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    PageTableEntry *page_tab      = (PageTableEntry *)(0xFFC00000 + dir_idx * PAGE_SIZE);
    page_tab[tab_idx]             = page_create_page_table_entry(0xDE, 0xDC0DE000 /* shifted 12 bits to the right */);
    page_tab[tab_idx].access_byte = 0xA;

    int is_empty = 1;
    // check if the page table is empty
    for (int i = 0; i < 1024; i++)
    {
        // looks like 0xDEADCODE
        if (!(page_tab[i].access_byte == 0xDE && page_tab[i].reserved == 0xA
              && page_tab[i].address_upper_20 == 0xDC0DE))
        {
            is_empty = 0;
            break;
        }
    }

    // if it is, we can safely free it
    if (is_empty)
    {
        KSLOG("found empty page table, freeing\n");
        pmm_free_page((void *)(PhysicalAddress)(g_pgdir[dir_idx].address_upper_20 << 12));
    }

    // flush the TLB entry, of course
    page_invalidate_page((void *)virt);
}

PhysicalAddress vmm_get_phys(VirtualAddress virt)
{
    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    PageTableEntry *page_tab = (PageTableEntry *)(0xFFC00000 + dir_idx * PAGE_SIZE);
    PhysicalAddress phys     = page_tab[tab_idx].address_upper_20 << 12;

    return phys;
}

uint32_t get_first_free_idx(uint32_t *bit)
{
    *bit = 0;

    for (uint32_t i = 0, base_addr = g_base; i < BITMAP_BITS; i++, base_addr += PAGE_SIZE)
    {
        uint32_t idx = i / 32;
        *bit         = i % 32;

        if (g_bitmap[idx] == 0xFFFFFFFF)
        {
            i += 31;
            continue;
        }

        if (!TESTBIT(g_bitmap[idx], (uint32_t)(1 << *bit)))
            return idx;
    }

    return UINT32_MAX; // this is impossible to reach
}
