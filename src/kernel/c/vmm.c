/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <page.h>
#include <pmm.h>
#include <serial.h>
#include <vmm.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>

#define PAGE_SIZE 4096

#define VADDR_GET_PAGE_DIR_IDX(vaddr) (((vaddr) >> 22) & 0x3FF)
#define VADDR_GET_PAGE_TAB_IDX(vaddr) (((vaddr) >> 12) & 0x3FF)
#define VADDR_GET_PAGE_FRM_OFF(vaddr) ((vaddr)&0xFFF)

extern uint8_t __start;
extern uint8_t __end;

static PageDirectoryEntry *g_page_dir;

void vmm_init()
{
    g_page_dir = pmm_allocate_page();

    uint32_t base = (uint32_t)(&__start);
    uint32_t top  = (uint32_t)(&__end);

    base = base / PAGE_SIZE * PAGE_SIZE;
    top  = ALIGN(top, PAGE_SIZE);

    PageTableEntry *page_tab = NULL;

    // let's identity map the kernel
    for (uint32_t i = base, dir_idx = 0, dir_idx_prev = 0xFFFF, tab_idx = 0; i < top; i += PAGE_SIZE)
    {
        dir_idx = VADDR_GET_PAGE_DIR_IDX(i);
        tab_idx = VADDR_GET_PAGE_TAB_IDX(i);

        if (dir_idx != dir_idx_prev)
        {
            KSLOG("allocating page for new page table\n");
            page_tab = pmm_allocate_page();

            KSLOG("creating new page directory entry\n");
            g_page_dir[dir_idx]
                = page_create_page_directory_entry(PGD_AX_PRESENT | PGD_AX_WRITE | PGD_AX_KERNEL, (uint32_t)(page_tab));
        }

        page_tab[tab_idx] = page_create_page_table_entry(PGT_AX_PRESENT | PGT_AX_WRITE | PGT_AX_KERNEL,
                                                         i /* mapping to the same address */);

        dir_idx_prev = dir_idx;
    }

    page_load_page_directory(g_page_dir);
    page_enable_paging();
}
