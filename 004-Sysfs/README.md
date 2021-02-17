## sysfs
---

### /proc
---
Runtime information dumped here. The numbered directories are the PID of the processes, and inside can see info about them:
```bash
LKM# ls /proc
[OUTPUT]
LKM#  ls /proc/10
[OUTPUT]
LKM#
```
### /sys (sysfs)
---
Organized structure for kernel objects. Checking for the `modparam.c` module from before, if surft the `/sys` folder structure:
```bash
LKM# cd /sys/module/modparam
LKM:/sys/module/modparam# ls
coresize  holders  initsize  initstate	notes  refcnt  sections  taint	uevent
LKM:/sys/module/modparam# 
```
This is important for the previous example, as the `@perm` variable for `module_param()` is the permission to set for this `/sys` folder. As the `@perm` is "0", the file is not created in `/sys`. This example replaces the "0" by defined permissions in the code.

### Code
---
```c
#define pr_fmt(fmt)     KBUILD_MODNAME "->%s:%d: " fmt, __func__ , __LINE__

#include <linux/module.h>
#include <linux/init.h>

static int counter;
module_param(counter, int, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(counter, "A Integer Param");

static char *message = NULL;
module_param(message, charp, S_IWUSR | SIRUGO);
MODULE_PARM_DESC(message, "A Message string PAram");

static int __init lkm_init(void)
{
        pr_info("Loading Module\n");
        pr_info("Param 1: counter: %d\n",counter);
        pr_info("Param 2: message: %s\n", message);
        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("LKM Example");
```
### include/uapi/linux/stat.h
---
Permissions for `@perm` can be:
```c
#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
```

### include/linux/stat.h
---
```c
#ifndef _LINUX_STAT_H
#define _LINUX_STAT_H

#include <asm/stat.h>
#include <uapi/linux/stat.h>

#define S_IRWXUGO       (S_IRWXU|S_IRWXG|S_IRWXO)
#define S_IALLUGO       (S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
#define S_IRUGO         (S_IRUSR|S_IRGRP|S_IROTH)
#define S_IWUGO         (S_IWUSR|S_IWGRP|S_IWOTH)
#define S_IXUGO         (S_IXUSR|S_IXGRP|S_IXOTH)
```
### Back to /sys
--- 
Now the module folder will contain the parameters in the file with the permissions defined in the code. Compile, and insmod the module, and then:
```bash
LKM# insmod helloworld.ko counter=1 message='"Hello World"'
LKM# cd /sys/module/mod_param/parameters/
LKM:/sys/module/mod_param/parameters# ls
counter  message
LKM:/sys/module/mod_param/parameters# cat counter 
1
LKM:/sys/module/mod_param/parameters# cat message 
Hello World
LKM:/sys/module/mod_param/parameters# 
```
This values can be modified for the module on runtime by placing the values directly to this files:
```bash
LKM:/sys/module/mod_param/parameters# cat 2 > counter
LKM:/sys/module/mod_param/parameters# cat "Message Here" > message
```
