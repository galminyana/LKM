## Hello World LKM
---

### Code
---
```c
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
First, the required header files are included for this basic LKM.
The `lkm_init` function is implemented. The `__init` macro, causes the init function to be discarded and its memory freed once the init function finishes. Inside this function, just a "Hello World" message is printed in the Kernel logs. The Init functions, return an integer: 
- If initialization was successful the function returns zero
- On failure, it returns nonzero.
```c
int my_init(void);
```
Then the `lkm_exit` funtion is implemented. The `__exit` macro causes the omission of the function when the module is built into the kernel. In this function, the "Bye World" message is printed in the KErnel logs. Exit functions might clean up resources, ensure that hardware is in a consistent state, and so on before returning. After the exit function returns, the module is unloaded.
```c
void my_exit(void);
```
After that, the functions `module_init()` and `module_exit()` macros are used to indicate the functions to run at module insertion/removal.
The rest of the `MODULE_*` macros are used to give some information about the module. But in the `MODULE_LICENSE` does not indicate a "GPL" license, the tainted flag is set in the Kernel.


