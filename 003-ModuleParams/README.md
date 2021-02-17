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
