/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxPAGEHxuuq
#define quuxPAGEHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum page_directory_access_flags
{
    PGD_AX_PRESENT      = 0b00000001,
    PGD_AX_READ         = 0b00000000,
    PGD_AX_WRITE        = 0b00000010,
    PGD_AX_USER         = 0b00000000,
    PGD_AX_KERNEL       = 0b00000100,
    PGD_AX_WRITE_THRU   = 0b00001000,
    PGD_AX_CACHE        = 0b00010000,
    PGD_AX_ACCESSED     = 0b00100000,
    PGD_AX_PAGE_SIZE_4K = 0b00000000,
    PGD_AX_PAGE_SIZE_4M = 0b10000000
};

enum page_table_access_flags
{
    PGT_AX_PRESENT  = 0b00000001,
    PGT_AX_READ     = 0b00000000,
    PGT_AX_WRITE    = 0b00000010,
    PGT_AX_USER     = 0b00000000,
    PGT_AX_KERNEL   = 0b00000100,
    PGT_AX_ACCESSED = 0b00100000,
    PGT_AX_DIRTY    = 0b01000000
};

struct __attribute__((packed)) page_directory_entry
{
    uint8_t  access_byte;
    uint8_t  reserved : 4;
    uint32_t address : 20;
};

struct __attribute__((packed)) page_table_entry
{
    uint8_t  access_byte;
    uint8_t  reserved : 4;
    uint32_t address : 20;
};

struct page_directory_entry page_create_page_directory_entry(uint8_t access_byte, uint32_t address);

struct page_table_entry page_create_page_table_entry(uint8_t access_byte, uint32_t address);

void page_load_page_directory(struct page_directory_entry *pgd);

void page_enable_paging(void);

#ifdef __cplusplus
}
#endif

#endif
