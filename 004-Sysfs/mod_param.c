#define pr_fmt(fmt)     KBUILD_MODNAME "->%s:%d: " fmt, __func__ , __LINE__

#include <linux/module.h>
#include <linux/init.h>

static int counter;                                     //<- This is 'counter' integer parameter
module_param(counter, int, S_IWUSR | S_IRUGO);          //<- Tells Kernel that 'counter' is a parameter
                                                        //   S_IWUSR | SIRUGO are the perms for the sysfs file
MODULE_PARM_DESC(counter, "A Integer Param");           //<- 'modinfo' description for the parameter


static char *message = NULL;                            //<- This is 'message' string parameter
module_param(message, charp, S_IWUSR | SIRUGO);         //<- Tells Kernel that 'message' is a parameter
                                                        //   S_IWUSR | SIRUGO are the perms for the sysfs file
MODULE_PARM_DESC(message, "A Message string PAram");    //<- 'modinfo' description for the parameter

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
