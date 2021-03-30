## Syscall Hooking: sys_chdir
---
For Kernel > 4.17.0

Intercept `sys_mkdir`. After this you should make a regular call (if it is not our directory) or return ENOTDIR (standing for 'there exists no directory with that name'). Now your tools should really be hidden from intermediate admins (advanced / paranoid ones will scan the HDD at its lowest level, but who is paranoid today besides us ?!). It should also be possible to defeat this HDD scan, because everything is based on systemcalls.

### `linux/syscalls.h` -> `sys_chdir`
---
Param is a pointer to the string containing the directory to change to
```c
asmlinkage long sys_chdir(const char __user *filename);
```
