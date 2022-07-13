/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxPAGEHxuuq
#define quuxPAGEHxuuq

#include <stdint.h>

#include "memdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The access flags of a page directory entry.
 */
typedef enum page_directory_access_flags
{
    PGD_AX_PRESENT      = 0b00000001,
    PGD_AX_READ         = 0b00000000,
    PGD_AX_WRITE        = 0b00000010,
    PGD_AX_KERNEL       = 0b00000000,
    PGD_AX_USER         = 0b00000100,
    PGD_AX_WRITE_THRU   = 0b00001000,
    PGD_AX_CACHE        = 0b00010000,
    PGD_AX_ACCESSED     = 0b00100000,
    PGD_AX_PAGE_SIZE_4K = 0b00000000,
    PGD_AX_PAGE_SIZE_4M = 0b10000000
} PageDirectoryAccessFlags;

/**
 * @brief The access flags of a page table entry.
 */
typedef enum page_table_access_flags
{
    PGT_AX_PRESENT  = 0b00000001,
    PGT_AX_READ     = 0b00000000,
    PGT_AX_WRITE    = 0b00000010,
    PGT_AX_KERNEL   = 0b00000000,
    PGT_AX_USER     = 0b00000100,
    PGT_AX_ACCESSED = 0b00100000,
    PGT_AX_DIRTY    = 0b01000000
} PageTableAccessFlags;

/**
 * @brief Represents a page directory entry, used by the Memory Management
 *        Unit.
 */
typedef struct __attribute__((packed)) page_directory_entry
{
    uint8_t         access_byte;
    uint8_t         reserved : 4;
    PhysicalAddress address_upper_20 : 20;
} PageDirectoryEntry;

/**
 * @brief Represents a page table entry, used by the Memory Management
 *        Unit.
 */
typedef struct __attribute__((packed)) page_table_entry
{
    uint8_t         access_byte;
    uint8_t         reserved : 4;
    PhysicalAddress address_upper_20 : 20;
} PageTableEntry;

/**
 * @brief Creates a single page directory entry with the specified attributes.
 *
 * @param access_byte The access flags of the entry.
 * @param address The (page-aligned) address of the page table entry that
 *                this entry points to.
 * @return The page directory entry created.
 */
PageDirectoryEntry page_create_page_directory_entry(uint8_t access_byte, PhysicalAddress address);

/**
 * @brief Creates a single page table entry with the specified attributes.
 *
 * @param access_byte The access flags of the entry.
 * @param address The address of the page frame that this entry points to.
 * @return The page table entry created.
 */
PageTableEntry page_create_page_table_entry(uint8_t access_byte, PhysicalAddress address);

/**
 * @brief Loads the specified page directory into the CR3 register.
 *
 * @param pgd The address of the page directory to load.
 */
void page_load_page_directory(PageDirectoryEntry *pgd);

/**
 * @brief Enables paging by setting the CR0.PG bit.
 */
void page_enable_paging(void);

/**
 * @brief Disables paging by doing the exact opposite that
 *        page_enable_paging does (unsets the CR0.PG bit).
 */
void page_disable_paging(void);

/**
 * @brief Reloads the whole CR3 register.
 */
void page_reload_cr3(void);

/**
 * @brief Invalidates a single page that may be cached in the TLB.
 *
 * @param page The address of the page to be invalidated.
 */
void page_invalidate_page(void *page);

#ifdef __cplusplus
}
#endif

#endif
