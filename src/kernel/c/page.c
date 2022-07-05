/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>

#include <page.h>
#include <pmm.h>

#define PAGE_SIZE 4096

struct page_directory_entry page_create_page_directory_entry(uint8_t access_byte, uint32_t address)
{
    struct page_directory_entry entry;
    entry.access_byte = access_byte;
    entry.address     = address & 0xFFFFF;

    return entry;
}

struct page_table_entry page_create_page_table_entry(uint8_t access_byte, uint32_t address)
{
    struct page_table_entry entry;
    entry.access_byte = access_byte;
    entry.address     = address & 0xFFFFF;

    return entry;
}

void page_load_page_directory(struct page_directory_entry *pgd)
{
    __asm__ __volatile__("movl %0, %%cr3" : : "a"((uint32_t)(pgd)));
}

void page_enable_paging()
{
    __asm__ __volatile__(
        "movl %cr0, %eax;"
        "orl $0x80000000, %eax;"
        "movl %eax, %cr0;");
}
