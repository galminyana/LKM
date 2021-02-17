## Module PArameters
---


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

/**
 * module_param_unsafe - same as module_param but taints kernel
 */
#define module_param_unsafe(name, type, perm)                   \
        module_param_named_unsafe(name, name, type, perm)
```