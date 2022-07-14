void mod_entry(void)
{
    __asm__ volatile("andl %eax, %eax");
    return;
}

void mod_exit(void)
{
    __asm__ volatile("nop");
    return;
}
