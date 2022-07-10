/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>

#include <memory/memdefs.h>
#include <memory/page.h>
#include <memory/pmm.h>

#define PAGE_SIZE 4096

struct page_directory_entry page_create_page_directory_entry(uint8_t access_byte, PhysicalAddress address)
{
    struct page_directory_entry entry;
    entry.access_byte      = access_byte;
    entry.address_upper_20 = (address & 0xFFFFF000) >> 12; // the address is supposed to be page-aligned
                                                           // so the lower 12 bits are all zeros, thus giving
                                                           // space for the access byte
    return entry;
}

struct page_table_entry page_create_page_table_entry(uint8_t access_byte, PhysicalAddress address)
{
    struct page_table_entry entry;
    entry.access_byte      = access_byte;
    entry.address_upper_20 = (address & 0xFFFFF000) >> 12;

    return entry;
}

void page_load_page_directory(struct page_directory_entry *pgd)
{
    __asm__ __volatile__("movl %0, %%cr3" : : "a"((uint32_t)(pgd)));
}

void page_enable_paging(void)
{
    __asm__ __volatile__(
        "movl %cr0, %eax;"
        "orl $0x80000000, %eax;"
        "movl %eax, %cr0;"); // enable paging bit
}

void page_disable_paging(void)
{
    __asm__ __volatile__(
        "movl %cr0, %eax;"
        "andl $0x7FFFFFFF, %eax;"
        "movl %eax, %cr0;");
}

void page_reload_cr3(void)
{
    __asm__ __volatile__(
        "movl %cr3, %ebx;"
        "movl %ebx, %cr3;");
}
