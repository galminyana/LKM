## Syscall Hooking: Kill
---
For Kernel > 4.17.0

Test by:

1) Build it
2) `insmod` the module
3) Do a `kill -64 9`. Nothing will happen
4) A `dmesg` will show the process and the signal sent
5) `rmmod` the module

