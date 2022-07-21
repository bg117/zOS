/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxVMMHxuuq
#define quuxVMMHxuuq

#include <stdint.h>

#include "memdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct page_directory_entry PageDirectoryEntry;

/**
 * @brief Initializes the virtual memory manager.
 *
 * @param pgd_phys The physical address of the initial page directory to use.
 * @param pgd_virt The virtual address of the initial page directory to use.
 */
void vmm_init(PhysicalAddress pgd_phys, VirtualAddress pgd_virt);

/**
 * @brief Allocates a single page of virtual memory.
 *
 * @return The allocated logical page.
 */
void *vmm_allocate_page(void);

/**
 * @brief Frees a page of virtual memory.
 *
 * @param page The page to free.
 */
void vmm_free_page(void *page);

/**
 * @brief Allocates n pages of contiguous virtual memory.
 *
 * @param n The amount of contiguous pages to allocate.
 * @return The base of the contiguous pages allocated.
 */
void *vmm_allocate_pages(int n);

/**
 * @brief Frees n pages starting from page_base.
 *
 * @param page_base The base of the contiguous virtual pages to free.
 * @param n The amount of pages to free.
 */
void vmm_free_pages(void *page_base, int n);

/**
 * @brief Maps the virtual address to the corresponding physical
 *        address.
 *
 * @param phys The physical address to map the virtual address to.
 * @param virt The virtual address to be mapped.
 */
void vmm_map_page(PhysicalAddress phys, VirtualAddress virt);

/**
 * @brief Unmaps the virtual address from the page directory/table.
 *
 * @param virt The virtual address/page to be unmapped.
 */
void vmm_unmap_page(VirtualAddress virt);

/**
 * @brief Gets the physical address that the virtual page is mapped to.
 *
 * @param virt The virtual address/page to find out its physical address.
 * @return The physical address that the virtual page is mapped to.
 */
PhysicalAddress vmm_get_phys(VirtualAddress virt);

/**
 * @brief Uses the specified page directory for mapping pages.
 *
 * @param pgd The page directory to use.
 */
void vmm_switch(PageDirectoryEntry *pgd);

#ifdef __cplusplus
}
#endif

#endif
