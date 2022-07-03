/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxPMMHxuuq
#define quuxPMMHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum page_status
{
    PS_FREE,
    PS_USED,
    PS_UNKNOWN
};

/**
 * @brief Initializes the physical memory manager.
 *
 * @param len The length of free memory available, starting from the base
              of the kernel (usually 0x100000).
 */
void pmm_init(uint64_t len);

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
 * @brief Gets the status of a page, whether free, used, or unknown.
 *
 * @param page The page to get the status of.
 * @return The status of the page. See enum page_status for reference.
 */
enum page_status pmm_get_page_status(void *page);

#ifdef __cplusplus
}
#endif

#endif
