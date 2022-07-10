/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxVMMHxuuq
#define quuxVMMHxuuq

#include <stdint.h>

#include <memory/memdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the virtual memory manager.
 */
void vmm_init(void);

/**
 * @brief Allocates a single page of virtual memory.
 */
void *vmm_allocate_page(void);

/**
 * @brief Frees a page of virtual memory.
 *
 * @param page The page to free.
 */
void vmm_free_page(void *page);

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
 * @return The physical address that the virtual page was mapped to.
 */
PhysicalAddress vmm_unmap_page(VirtualAddress virt);

#ifdef __cplusplus
}
#endif

#endif
