/*
 Author: Guillem Alminyana
 License: GPL v3

 Example of printk use, and the pr_* equivalent

*/

#define pr_fmt(fmt)	KBUILD_MODNAME "->%s:%d: " fmt, __func__, __LINE__

#include <linux/module.h>
#include <linux/init.h>

static int __init lkm_init(void)
{
	printk(KERN_INFO "This is KERN_INFO log level\n");
	pr_info("Hello Info\n");

	printk(KERN_ALERT "This is KERN_ALERT log level\n");
	pr_alert("Hello Alert\n");

	printk(KERN_EMERG "This is KERN_EMERG log level\n");
	pr_emerg("Hello Emergency\n");

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
