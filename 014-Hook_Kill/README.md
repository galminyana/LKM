## Syscall Hooking: Kill
---
For Kernel > 4.17.0

### Kill Syscall
---
```c
asmlinkage long sys_kill(pid_t pid, int sig);
```
Where PID is the process ID and sig stands for the signal to send the process
As going to use the `pt_regs`, the function will be defined in the code as:
```c
asmlinkage int (*original_kill) (const struct pt_regs *regs);
```
Where `regs` will store the params as:
- RDI <- for the PID
- RSI <- for the signal
In the hooked function, need to get the params from the `pt_regs` struct defined.

### Test
---
1) Build it
2) `insmod` the module
3) Do a `kill -64 9`. Nothing will happen
4) A `dmesg` will show the process and the signal sent
5) `rmmod` the module
