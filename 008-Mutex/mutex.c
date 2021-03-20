#include <linux/module.h>
#include <linux/init.h>

static DEFINE_MUTEX(my_mutex);

static int __init lkm_init(void)
{
        pr_info("Loading Module\n");

        pr_info("   Initializing mutex..\n");
        mutex_init(&my_mutex);          //<- Mutex initialization
        pr_info("   Mutex Initialized.\n");

        pr_info("   mutex_is_locked: %d\n.", mutex_is_locked(&my_mutex));

        pr_info("   Locking Mutex...\n");
        mutex_lock(&my_mutex);
        pr_info("   Mutex Locked.\n");

        pr_info("   mutex_is_locked: %d\n.", mutex_is_locked(&my_mutex));

        pr_info("   Releasing mutex...");
        mutex_unlock(&my_mutex);
        pr_info("   Mutex unlocked.\n");

	pr_info("   mutex_is_locked: %d\n.", mutex_is_locked(&my_mutex));

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Destroying Mutex.\n");
        mutex_destroy(&my_mutex);

        pr_info("Exiting... \n");
}

module_init(lkm_init);          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Mutex Example");
