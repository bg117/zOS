void mod_entry(void)
{
    asm volatile("andl %eax, %eax");
    return;
}

void mod_exit(void)
{
    asm volatile("nop");
    return;
}
