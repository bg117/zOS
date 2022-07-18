/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxPMMHxuuq
#define quuxPMMHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents the status of a physical page.
 */
typedef enum page_status
{
    PS_FREE,
    PS_USED,
    PS_UNKNOWN
} PageStatus;

typedef struct memory_map_entry MemoryMapEntry;

/**
 * @brief Initializes the physical memory manager.
 *
 * @param mmap The memory map of the system.
 * @param mmap_length Tells how many entries the memory map contains.
 */
void pmm_init(MemoryMapEntry *mmap, size_t mmap_length);

/**
 * @brief Allocates a single page and marks it as used.
 *
 * @return The allocated page. Commonly 4096 bytes.
 */
void *pmm_allocate_page(void);

/**
 * @brief Frees the specified page.
 *
 * @param page The page to be freed. Must be page-aligned.
 */
void pmm_free_page(void *page);

/**
 * @brief Allocates n pages and marks them as used.
 *
 * @return The allocated page. 4096 bytes multiplied by n.
 */
void *pmm_allocate_pages(int n);

/**
 * @brief Frees the specified pages.
 *
 * @param page_base The base address of the pages to be freed.
 *                  Must be page-aligned.
 */
void pmm_free_pages(void *page_base, int n);

/**
 * @brief Gets the status of a page, whether free, used, or unknown.
 *
 * @param page The page to get the status of.
 * @return The status of the page. See enum page_status for reference.
 */
PageStatus pmm_get_page_status(void *page);

/**
 * @brief Gets the size of the bitmap which keeps track of free/used pages.
 *
 * @return The size of the bitmap (in bytes).
 */
uint64_t pmm_get_bitmap_length(void);

#ifdef __cplusplus
}
#endif

#endif
