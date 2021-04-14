#include <linux/module.h>
#include <linux/kernel.h>



static int __init lkm_init(void)
{
        pr_info("Loading Module.\n");

 
        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Task Credentials Example");
