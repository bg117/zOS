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

void vmm_init(void);

void *vmm_allocate_page(void);

void vmm_free_page(void *page);

#ifdef __cplusplus
}
#endif

#endif
