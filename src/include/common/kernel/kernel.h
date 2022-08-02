/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxKERNELHxuuq
#define quuxKERNELHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct memory_map_entry MemoryMapEntry;

static const char *const EMPTY_PANIC_MSG = "\033;0\033;" /* ??? what */;

/**
 * @brief Initializes the kernel.
 *
 * @param mmap The memory map to use.
 * @param mmap_length The length of the memory map.
 */
void kernel_init(MemoryMapEntry *mmap, size_t mmap_length);

/**
 * @brief Deinitializes the kernel, modules, and the associated
 *        hardware.
 */
void kernel_deinit(void);

/**
 * @brief Throws a kernel panic.
 *
 * @param file The file where the panic was thrown.
 * @param line The line number in which the panic was thrown.
 * @param msg (Optional) The message to display. Pass EMPTY_PANIC_MSG to
 *            disregard.
 * @param ... The arguments to use with the format string.
 */
void kernel_panic(const char *file, int line, const char *fmt, ...);

#define PANIC(msg, ...) kernel_panic(__FILE__, __LINE__, msg, ##__VA_ARGS__)
#define NPANIC()        PANIC(EMPTY_PANIC_MSG)

#ifdef __cplusplus
}
#endif

#endif
