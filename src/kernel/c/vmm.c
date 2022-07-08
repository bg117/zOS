/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <memory/mem.h>
#include <memory/page.h>
#include <memory/pmm.h>
#include <memory/vmm.h>

#include <hw/serial.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>

#define PAGE_SIZE 4096

#define VADDR_GET_PAGE_DIR_IDX(vaddr) (((vaddr) >> 22) & 0x3FF)
#define VADDR_GET_PAGE_TAB_IDX(vaddr) (((vaddr) >> 12) & 0x3FF)
#define VADDR_GET_PAGE_FRM_OFF(vaddr) ((vaddr)&0xFFF)

extern uint8_t __start;
extern uint8_t __end;

static PageDirectoryEntry *g_page_dir;
static PageDirectoryEntry *g_virt_page_dir = (PageDirectoryEntry *)(0xFFC00000);

void vmm_init()
{
    g_page_dir = pmm_allocate_page();
    mem_fill(g_page_dir, 0, PAGE_SIZE);

    uint32_t base = 0;
    uint32_t top  = (uint32_t)(&__end) + pmm_get_bitmap_length();

    top = ALIGN(top, PAGE_SIZE);

    PageTableEntry *page_tab = NULL;

    // let's identity map the kernel space
    for (uint32_t i = base, dir_idx = 0, dir_idx_prev = 0xFFFF, tab_idx = 0; i < top; i += PAGE_SIZE)
    {
        dir_idx = VADDR_GET_PAGE_DIR_IDX(i);
        tab_idx = VADDR_GET_PAGE_TAB_IDX(i);

        if (dir_idx != dir_idx_prev)
        {
            KSLOG("allocating page for new page table\n");
            page_tab = pmm_allocate_page();
            mem_fill(page_tab, 0, PAGE_SIZE);

            KSLOG("creating new page directory entry\n");
            g_page_dir[dir_idx]
                = page_create_page_directory_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL, (uint32_t)(page_tab));
        }

        page_tab[tab_idx] = page_create_page_table_entry(PGT_AX_PRESENT | PGT_AX_WRITE | PGT_AX_KERNEL,
                                                         i /* mapping to the same address */);

        dir_idx_prev = dir_idx;
    }

    // recursive page directory
    g_page_dir[1023]
        = page_create_page_directory_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL, (uint32_t)(g_page_dir));

    page_load_page_directory(g_page_dir);
    page_enable_paging();
}

void *vmm_allocate_page()
{
    void *page = pmm_allocate_page();
    return page;
}

void vmm_free_page(void *page)
{
    pmm_free_page(page);
}
