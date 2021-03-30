## Syscall Hooking: sys_chdir
---
Intercept `sys_mkdir`. For Kernel > 4.17.0


### `linux/syscalls.h` -> `sys_chdir`
---
Param is a pointer to the string containing the directory to change to
```c
asmlinkage long sys_chdir(const char __user *filename);
```
To get the `filename` string length, use the `strlen()` function.

