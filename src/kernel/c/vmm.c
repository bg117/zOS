/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdbool.h>

#include <kernel/memory/memdefs.h>
#include <kernel/memory/page.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>

#include <kernel/hw/serial.h>

#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log_macros.h>

#include <utils/mem.h>

#define PAGE_SIZE               0x1000
#define VIRTUAL_BASE            0xC0000000
#define MAGIC_NUMBER            0xDEADC0DE
#define RECURSIVE_PAGE_TAB_BASE 0xFFC00000

#define VADDR_GET_PAGE_DIR_IDX(vaddr) (((vaddr) >> 22) & 0x3FF)
#define VADDR_GET_PAGE_TAB_IDX(vaddr) (((vaddr) >> 12) & 0x3FF)
#define VADDR_GET_PAGE_FRM_OFF(vaddr) ((vaddr)&0xFFF)

#define GET_RECURSIVE_PAGE_TAB(dir_idx) (RECURSIVE_PAGE_TAB_BASE + (dir_idx)*PAGE_SIZE)

extern uint32_t SYS_PGDIR;

static PageDirectoryEntry *g_pgdir;

static VirtualAddress get_free_base(int n);

void vmm_init(PhysicalAddress pgd_phys, VirtualAddress pgd_virt)
{
    page_load_page_directory((PageDirectoryEntry *)pgd_phys);
    g_pgdir = (PageDirectoryEntry *)pgd_virt;
}

void vmm_switch(PageDirectoryEntry *pgd)
{
    KSLOG("switching to new page directory located at 0x%08X\n", (PhysicalAddress)vmm_get_phys((VirtualAddress)pgd));

    page_load_page_directory((PageDirectoryEntry *)vmm_get_phys((VirtualAddress)pgd));
    g_pgdir = pgd;
}

void *vmm_allocate_page()
{
    return vmm_allocate_pages(1);
}

void vmm_free_page(void *page)
{
    vmm_free_pages(page, 1);
}

void *vmm_allocate_pages(int n)
{
    if (n == 0)
    {
        KSLOG("error: trying to allocate 0 pages\n");
        return (void *)MAGIC_NUMBER;
    }

    KSLOG("allocating %d page%s\n", n, n > 1 ? "s" : "");

    VirtualAddress base = get_free_base(n), addr = base;

    if (base == MAGIC_NUMBER)
    {
        KSLOG("error: cannot find %d %s\n", n, n > 1 ? "contiguous pages" : "page");
        return (void *)MAGIC_NUMBER;
    }

    for (int i = 0; i < n; i++, base += PAGE_SIZE)
        vmm_map_page((PhysicalAddress)pmm_allocate_page(), base);

    return (void *)addr;
}

void vmm_free_pages(void *page_base, int n)
{
    if (n == 0)
    {
        KSLOG("error: trying to free 0 pages\n");
        return;
    }

    KSLOG("freeing %d page%s\n", n, n > 1 ? "s" : "");

    VirtualAddress base = (VirtualAddress)page_base;
    for (int i = 0; i < n; i++, base += PAGE_SIZE)
    {
        pmm_free_page((void *)vmm_get_phys(base));
        vmm_unmap_page(base);
    }
}

void vmm_map_page(PhysicalAddress phys, VirtualAddress virt)
{
    KSLOG("mapping page 0x%08X to physical address 0x%08X\n", virt, phys);

    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    if (!TESTBIT(g_pgdir[dir_idx].access_byte, PGD_AX_PRESENT))
    {
        KSLOG("page table not present, creating one\n");

        void *page_tab   = pmm_allocate_page();
        g_pgdir[dir_idx] = page_create_page_directory_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL,
                                                            (PhysicalAddress)page_tab);
    }

    PageTableEntry *page_tab = (PageTableEntry *)GET_RECURSIVE_PAGE_TAB(dir_idx);
    if (TESTBIT(page_tab[tab_idx].page_frame_status, PGF_STATUS_USED))
    {
        KSLOG("error: trying to map physical 0x%08X to used page 0x%08X\n", phys, virt);
        return;
    }

    page_tab[tab_idx] = page_create_page_table_entry(
        PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL, PGF_STATUS_USED, phys & 0xFFFFF000);
}

void vmm_unmap_page(VirtualAddress virt)
{
    if (virt == 0x9000)
        __asm__ volatile("nop");

    KSLOG("unmapping page 0x%08X\n", virt);
    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    PageTableEntry *page_tab      = (PageTableEntry *)GET_RECURSIVE_PAGE_TAB(dir_idx);
    VirtualAddress  page_tab_virt = (VirtualAddress)page_tab;
    PageTableEntry *ent           = &page_tab[tab_idx];

    if (!TESTBIT(page_tab[tab_idx].page_frame_status, PGF_STATUS_USED))
        KSLOG("warning: address 0x%08X already unmapped, ignoring\n", virt);
    else
        UNSETBITVAR(page_tab[tab_idx].page_frame_status, PGF_STATUS_USED);

    bool is_empty = true;
    // check if the page table is empty
    for (int i = 0; i < 1024; i++)
    {
        if (TESTBIT(page_tab[i].page_frame_status, PGF_STATUS_USED))
        {
            is_empty = false;
            break;
        }
    }

    // if it is, we can safely free it
    if (is_empty)
    {
        KSLOG("found empty page table, freeing\n");
        pmm_free_page((void *)(PhysicalAddress)(g_pgdir[dir_idx].address_upper_20 << 12));
        g_pgdir[dir_idx] = page_create_page_directory_entry(0, 0);
    }

    // flush the TLB entry, of course
    page_invalidate_page((void *)virt);
}

PhysicalAddress vmm_get_phys(VirtualAddress virt)
{
    uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(virt);
    uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(virt);

    PageTableEntry *page_tab = (PageTableEntry *)GET_RECURSIVE_PAGE_TAB(dir_idx);
    PhysicalAddress phys     = page_tab[tab_idx].address_upper_20 << 12;

    return phys;
}

VirtualAddress get_free_base(int n)
{
    for (VirtualAddress lim = 0; lim < VIRTUAL_BASE;)
    {
        uint32_t dir_idx = VADDR_GET_PAGE_DIR_IDX(lim);
        uint32_t tab_idx = VADDR_GET_PAGE_TAB_IDX(lim);

        PageTableEntry *page_tab = (PageTableEntry *)GET_RECURSIVE_PAGE_TAB(dir_idx);

        if (TESTBIT(page_tab[tab_idx].page_frame_status, PGF_STATUS_USED))
        {
            lim += PAGE_SIZE;
            continue;
        }

        VirtualAddress init_addr = lim;
        bool           found     = true;
        for (int i = 0; i < n; i++, lim += PAGE_SIZE)
        {
            dir_idx = VADDR_GET_PAGE_DIR_IDX(lim);
            tab_idx = VADDR_GET_PAGE_TAB_IDX(lim);

            page_tab = (PageTableEntry *)GET_RECURSIVE_PAGE_TAB(dir_idx);

            if (TESTBIT(page_tab[tab_idx].page_frame_status, PGF_STATUS_USED))
            {
                found = false;
                break;
            }
        }

        if (found)
            return init_addr;
    }

    return MAGIC_NUMBER;
}
