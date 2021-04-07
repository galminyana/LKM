#include <linux/module.h>
#include <linux/init.h>



static int __init lkm_init(void)
{


        return 0;
}

static void __exit lkm_exit(void)
{


        pr_info("Exiting... \n");
}

module_init(lkm_init);          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Task Scheduler Example");
