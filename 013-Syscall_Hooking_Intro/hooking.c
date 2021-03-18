#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>



static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

 
        pr_info("Done initializing module.\n", DEVICE_NAME);
        return 0;
}

static void __exit lkm_exit(void)
{
 
        pr_info("Exiting... Bye.\n");
}

module_init(lkm_init);                                                                          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);                                                                          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("LKM Char Device Example");


