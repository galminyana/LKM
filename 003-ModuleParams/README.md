## Module PArameters
---
### Code
---
```c
#define pr_fmt(fmt)     KBUILD_MODNAME "->%s:%d: " fmt, __func__ , __LINE__

#include <linux/module.h>
#include <linux/init.h>

// Var are defined as usual but then have to define that's
// a variable by using the 'module_param()'
// To give a description of the param in the 'modinfo' can use
// the macro MODULE_PARAM_DESC()
static int counter;
module_param(counter, int, 0);
MODULE_PARM_DESC(counter, "A Integer Param");

static char *message = NULL;
module_param(message, charp, 0);
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
Once compiled the module, if it's `insmod` without params, the result is:
```bash
LKM# insmod helloworld.ko 
LKM# dmesg
[ 1270.811605] helloworld: module license 'GPL v3' taints kernel.
[ 1270.811608] Disabling lock debugging due to kernel taint
[ 1270.812684] helloworld->lkm_init:30: Loading Module
[ 1270.812687] helloworld->lkm_init:32: Param 1: counter: 0
[ 1270.812689] helloworld->lkm_init:34: Param 2: message: (null)
LKM# 
```
To define value for the params, the `insmod` must specify the params this way:
```bash
LKM# insmod helloworld.ko counter=1 message="Hello World"
LKM# dmesg
[ 1425.557616] helloworld: unknown parameter 'World' ignored
[ 1425.557692] helloworld->lkm_init:30: Loading Module
[ 1425.557694] helloworld->lkm_init:32: Param 1: counter: 1
[ 1425.557696] helloworld->lkm_init:34: Param 2: message: Hello
LKM# 
```
But notice, that the "World" part has not been printed in the logs. To avoid this, the `insmod` must put single quotes to close the entire string and the double quotes:
```bash
LKM# insmod helloworld.ko counter=1 message='"Hello World"'
LKMs# dmesg
[ 1796.392891] helloworld->lkm_init:30: Loading Module
[ 1796.392894] helloworld->lkm_init:32: Param 1: counter: 1
[ 1796.392896] helloworld->lkm_init:34: Param 2: message: Hello World
LKM# 
```
To check the params, can use `modinfo`. Will tell which params and the type of them:
```bash
LKM# modinfo helloworld
modinfo: ERROR: Module helloworld not found.
root@debian:~/LKM/007-module_params# modinfo helloworld.ko 
filename:       /root/LKM/007-module_params/helloworld.ko
description:    LKM Example
license:        GPL v3
author:         Guillem Alminyana
depends:        
retpoline:      Y
name:           helloworld
vermagic:       4.19.0-14-amd64 SMP mod_unload modversions 
parm:           counter:A Integer Param (int)
parm:           message:A Message string PAram (charp)
LKM# 
```
### linux/moduleparam.h
---
```c
/**
 * module_param - typesafe helper for a module/cmdline parameter
 * @value: the variable to alter, and exposed parameter name.
 * @type: the type of the parameter
 * @perm: visibility in sysfs.
 *
 * @value becomes the module parameter, or (prefixed by KBUILD_MODNAME and a
 * ".") the kernel commandline parameter.  Note that - is changed to _, so
 * the user can use "foo-bar=1" even for variable "foo_bar".
 *
 * @perm is 0 if the the variable is not to appear in sysfs, or 0444
 * for world-readable, 0644 for root-writable, etc.  Note that if it
 * is writable, you may need to use kernel_param_lock() around
 * accesses (esp. charp, which can be kfreed when it changes).
 *
 * The @type is simply pasted to refer to a param_ops_##type and a
 * param_check_##type: for convenience many standard types are provided but
 * you can create your own by defining those variables.
 *
 * Standard types are:
 *      byte, short, ushort, int, uint, long, ulong
 *      charp: a character pointer
 *      bool: a bool, values 0/1, y/n, Y/N.
 *      invbool: the above, only sense-reversed (N = true).
 */
#define module_param(name, type, perm)                          \
        module_param_named(name, name, type, perm)
```
```c
/**
 * module_param_array - a parameter which is an array of some type
 * @name: the name of the array variable
 * @type: the type, as per module_param()
 * @nump: optional pointer filled in with the number written
 * @perm: visibility in sysfs
 *
 * Input and output are as comma-separated values.  Commas inside values
 * don't work properly (eg. an array of charp).
 *
 * ARRAY_SIZE(@name) is used to determine the number of elements in the
 * array, so the definition must be visible.
 */
#define module_param_array(name, type, nump, perm)              \
        module_param_array_named(name, name, type, nump, perm)
```
```c
/**
 * module_param_string - a char array parameter
 * @name: the name of the parameter
 * @string: the string variable
 * @len: the maximum length of the string, incl. terminator
 * @perm: visibility in sysfs.
 *
 * This actually copies the string when it's set (unlike type charp).
 * @len is usually just sizeof(string).
 */
#define module_param_string(name, string, len, perm)                    \
        static const struct kparam_string __param_string_##name         \
                = { len, string };                                      \
        __module_param_call(MODULE_PARAM_PREFIX, name,                  \
                            &param_ops_string,                          \
                            .str = &__param_string_##name, perm, -1, 0);\
        __MODULE_PARM_TYPE(name, "string")
```
