/*
 Author: Guillem Alminyana
 License: GPL v3
 Library with usefull functions to deal with memory in Linux Kernel Space
*/
#include <linux/kernel.h>
#include <asm/paravirt.h>

/*
 Reads the Value of CR0
 Returns value of CR0
*/
static inline unsigned long my_read_cr0(void)
{
        return read_cr0();
}

/*
 Writes the value to the CR0 register
 @value: value to apply to CO0
*/
static inline void my_write_cr0(unsigned long value)
{
    unsigned long __force_order;
    asm volatile("mov %0, %%cr0" : "+r"(value), "+m"(__force_order));
}

/*
 Protects Kernel Space Memory making it Read Only. RO
*/
static inline void my_memory_ro(void)
{
    my_write_cr0(read_cr0() | 0x10000);
}

/*
 Makes Kernel Space Memory Writable. RW
*/
static inline void my_memory_rw(void)
{
    my_write_cr0(read_cr0() & ~0x10000);
}
