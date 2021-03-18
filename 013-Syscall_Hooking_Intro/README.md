## Syscall Hooking: Introduction
---
For Kernel > 4.17.0

All syscalls are definet in `linux/syscalls.h`.

### Linux Kernel Syscalls Handling
---
The params are passed throught CPU regs (SLAE64 notes explains the concept), and Kernel puts them all into a struct called `pt_regs` defined in the `ptrace.h` file for the architecture. For ARM_64:

```c
struct pt_regs {
    unsigned long bx;
    unsigned long cx;
    unsigned long dx;
    unsigned long si;
    unsigned long di;
    /* redacted for clarity */
};
```
The original syscalls take the arguments from this struct. In case that want to hook a syscall, need to care to get the params fom this structure.