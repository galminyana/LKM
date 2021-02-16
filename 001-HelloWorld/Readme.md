## Hello World LKM
---

### Code
---
```c
#define pr_fmt(fmt)	KBUILD_MODNAME "->%s:%d: " fmt, __func__, __LINE__

#include <linux/module.h>
#include <linux/init.h>

static int __init lkm_init(void)
{
	pr_info("Hello World\n");         //<- \n is required at the end
	return 0;
}

static void __exit lkm_exit(void)
{
	pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);		//<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);		//<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("LKM Example");
```

The `lkm_init` function is implemented. The `__init` macro, causes the init function to be discarded and its memory freed once the init function finishes. Inside this function, just a "Hello World" message is printed in the Kernel logs. The initialization function returns: 
- 0 if the initialization was succesful
- non 0 id a failure
```c
int my_init(void);
```
The `lkm_exit` funtion is implemented. The `__exit` macro causes the omission of the function when the module is built into the kernel. In this function, the "Bye World" message is printed in the Kernel logs. Exit functions might clean up resources and so on before returning. After the exit function returns, the module is unloaded.
```c
void my_exit(void);
```
The `module_init()` and `module_exit()` are used to indicate the functions to run at module insertion/removal.
The rest of the `MODULE_*` macros are used to give some information about the module. If the `MODULE_LICENSE` is not set to "GPL", the tainted flag is set in the Kernel.

Also some special format is defined for the printed messages _overwriting_ to `pr_fmt()` at:
```c
#define pr_fmt(fmt)	KBUILD_MODNAME "->%s:%d: " fmt, __func__, __LINE__
```
- `__func__` is for the function name where the printing is done
- `__LINE__` the line number where the printing is done

This gives the following format on log messages and printed messages:
```markup
LKM# tail /var/log/kern.log -n 0 -f
Feb 16 23:01:51 debian kernel: [  537.197758] helloworld: module license 'GPL v3' taints kernel.
Feb 16 23:01:51 debian kernel: [  537.197760] Disabling lock debugging due to kernel taint
Feb 16 23:01:51 debian kernel: [  537.198931] helloworld->__lkm_init:27:__ Hello World
Feb 16 23:01:58 debian kernel: [  544.085541] helloworld->__lkm_exit:33:__ Exiting... Bye World
LKM#
```
### Makefile
---
```c
obj-m += helloworld.o
modules:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) modules
clean:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) clean 
```

To compile the module, the `KERNEL_ROOT` parameter needs to be specified. In the case of Debian Kernel Headers, the path is `/lib/modules/4.19.0-14-amd64/build`. Hence, to compile, the complete command must be:
```bash
LKM# KERNEL_ROOT=/lib/modules/4.19.0-14-amd64/build make
make[1]: se entra en el directorio '/usr/src/linux-headers-4.19.0-14-amd64'
  CC [M]  /root/LKM/004-HelloWorld/helloworld.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /root/LKM/004-HelloWorld/helloworld.mod.o
  LD [M]  /root/LKM/004-HelloWorld/helloworld.ko
make[1]: se sale del directorio '/usr/src/linux-headers-4.19.0-14-amd64'
LKM#
```

To clean:
```bash
LKM# KERNEL_ROOT=/lib/modules/4.19.0-14-amd64/build make clean
make[1]: se entra en el directorio '/usr/src/linux-headers-4.19.0-14-amd64'
  CLEAN   /root/LKM/004-HelloWorld/.tmp_versions
  CLEAN   /root/LKM/004-HelloWorld/Module.symvers
make[1]: se sale del directorio '/usr/src/linux-headers-4.19.0-14-amd64'
LKM#
```

### Results
---
Once the module is compiled, it generates several files. The module file itself is the one with the **.ko** extension.

To get module information:
```bash
LKM# modinfo helloworld.ko 
filename:       /root/LKM/004-HelloWorld/helloworld.ko
description:    LKM Example
license:        GPL v3
author:         Guillem Alminyana
depends:        
retpoline:      Y
name:           helloworld
vermagic:       4.19.0-14-amd64 SMP mod_unload modversions 
LKM# 
``` 
To load the module into the Kernel:
```bash
LKM# insmod helloworld.ko 
LKM# lsmod|grep helloworld
helloworld             16384  0
LKM#
```
To remove the module:
```bash
LKM# rmmod helloworld
```
### Reviewing Module Log Printed Messages
---
This can be checked with `dmesg`, however, it will print lot of information from the Kernel among ith the module logs:
```bash
LKM# dmesg
...REMOVED...
[ 1297.197873] helloworld: module license 'GPL v3' taints kernel.
[ 1297.197876] Disabling lock debugging due to kernel taint
[ 1297.199031] Hello World
[ 1373.750760] Exiting... Bye World
```
A trick:
```bash
LKM# tail -f /var/log/kern.log -n 0
Feb 13 13:00:00 debian kernel: [ 1717.082601] Hello World		<== Message when `insmod` the module
Feb 13 13:00:04 debian kernel: [ 1721.761415] Exiting... Bye World	<== Message when `rmmod` the module
^C
LKM#
```
