/*
 Author: Guillem Alminyana
 License: GPL v3

 Basic example of a LKM: 
   - When the module is loaded, prints the "Hello World" in the kernel logs.
   - When the module is removed, prints in the logs the "Bye World" text

 * Two functions declared that are called when module is loaded/removed
 * The 'pr_info' uses the 'printk' from the kernel to log the text with information log level
 * 'module_init' and 'module_exit' macros are to define the init and exit functions
 * The 'MODULE_*' macros are informational for the Kernel

   */

#define pr_fmt(fmt)	KBUILD_MODNAME "->%s:%d: " fmt, __func__, __LINE__

#include <linux/module.h>
#include <linux/init.h>


static int __init lkm_init(void)
{
	// Uses print_k with log level information
	// The message is printed in the kernel log messages
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
