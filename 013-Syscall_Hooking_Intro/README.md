## Syscall Hooking: Introduction
---
For Kernel > 4.17.0

All syscalls are definet in `linux/syscalls.h`.

### Syscall Table
---
`sys_call_table` holds a pointer to all the available syscalls, and it's defined in `/arch/arc/kernel/sys.c`. 
```c 
void *sys_call_table[NR_syscalls] = {
	[0 ... NR_syscalls-1] = sys_ni_syscall,
#include <asm/unistd.h>
};
```
Where the max number of syscalls are defined in Kernel.
```c
#define __NR_syscalls 292
#define NR_syscalls (__NR_syscalls)
```
The array is initialized with `sys_ni_call` value, and non implemented syscalls are redirected to this function
```c 
asmlinkage long sys_ni_syscall(void)
{
	return -ENOSYS;
}
```
### Search for Syscall Array
---
There are various techniques, but the easier is to use `kallsyms_lookup_name`, defined at `linux/kallsyms.h`, that searches memory for the given symbol name and returns the address where is located
```c
/* Lookup the address for a symbol. Returns 0 if not found. */
unsigned long kallsyms_lookup_name(const char *name);
```
### Hook Syscall
--- 
Just need to replace the Syscall Table entry with the pointer to our implemented syscall to replace the original one. Good idea to save original memory address for the original syscall. 
The function implemented to replace the syscall must respect the params. for that, check definition of the syscall to replace at `linux/syscalls.h`.

### Linux Kernel Syscalls Handling
---
The params are passed throught CPU regs (SLAE64 notes explains the concept), and Kernel puts them all into a struct called `pt_regs` defined in the `ptrace.h` file for the architecture. For ARM_64:

```c
struct pt_regs {
    unsigned long bx;
    unsigned long cx;
    unsigned long dx;		//<- 3th Param
    unsigned long si;		//<- 2nd Param
    unsigned long di;		//<- 1st Param
};
```
The original syscalls take the arguments from this struct. In case that want to hook a syscall, need to care to get the params fom this structure.

### Write in RO Memory
---
Before hooking the syscall, need to make memory writable. For that, the `WP` flag in the `cr0` register must be `0`. With this functions can do:
```c
static unsigned long my_cr0;

static inline void my_write_cr0(unsigned long val)
{
    unsigned long __force_order;
    asm volatile("mov %0, %%cr0" : "+r"(val), "+m"(__force_order));
}
static inline void my_memory_ro(void)
{
    my_write_cr0(my_cr0);
}

static inline void my_memory_rw(void)
{
    my_write_cr0(my_cr0 & ~0x00010000);
}
```
For using, need to define a global `unsigned long` for the `cr0` value and read it using this Kernel function:
```c
unsigned long (*read_cr0)(void);
```
