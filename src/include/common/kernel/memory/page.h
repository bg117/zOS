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
    PGD_AX_PRESENT      = 0x01,
    PGD_AX_READ         = 0x00,
    PGD_AX_WRITE        = 0x02,
    PGD_AX_KERNEL       = 0x00,
    PGD_AX_USER         = 0x04,
    PGD_AX_WRITE_THRU   = 0x08,
    PGD_AX_CACHE        = 0x10,
    PGD_AX_ACCESSED     = 0x20,
    PGD_AX_PAGE_SIZE_4K = 0x00,
    PGD_AX_PAGE_SIZE_4M = 0x80
} PageDirectoryAccessFlags;

/**
 * @brief The access flags of a page table entry.
 */
typedef enum page_table_access_flags
{
    PGT_AX_PRESENT  = 0x01,
    PGT_AX_READ     = 0x00,
    PGT_AX_WRITE    = 0x02,
    PGT_AX_KERNEL   = 0x00,
    PGT_AX_USER     = 0x04,
    PGT_AX_ACCESSED = 0x20,
    PGT_AX_DIRTY    = 0x40
} PageTableAccessFlags;

/**
 * @brief Represents the status of a page frame.
 */
typedef enum page_frame_status_flags
{
    PGF_STATUS_USED = 0x01,
    PGF_STATUS_FREE = 0x00
} PageFrameStatusFlags;

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
    uint8_t access_byte;

    /* originally the 'AVAILABLE' half-byte, we
       repurpose it to contain the status of the page frame */
    uint8_t         page_frame_status : 4;
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
 * @param status The status of the page frame the entry points to.
 * @param address The address of the page frame that this entry points to.
 * @return The page table entry created.
 */
PageTableEntry page_create_page_table_entry(uint8_t access_byte, PageFrameStatusFlags status, PhysicalAddress address);

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
