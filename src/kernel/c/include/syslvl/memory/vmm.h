/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxVMMHxuuq
#define quuxVMMHxuuq

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

#ifdef __cplusplus
}
#endif

#endif
