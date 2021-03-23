#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/syscalls.h>

#include "my_memory.h"

static unsigned long *syscall_table;                            //<- Pointer to the Kernel syscall table

asmlinkage int (*original_shutdown) (int, int);                 //<- Define the original syscall


asmlinkage int hooked_shutdown(int i, int z)
{
        pr_info("   Hooked Syscall!!\n");
        //return orig_shutdown(i, z);
        return 0;
}

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        syscall_table = (void *) kallsyms_lookup_name("sys_call_table");                //<- Get address for Kernel Syscall Table

        if (sys_call_table == NULL) {
                pr_err("   Can't find syscall table!.\n");
                return -1;
        }

        original_shutdown = (void *) syscall_table[__NR_shutdown];      //<- Save actual address for sys_shutdown

        my_memory_rw();                                 //<- Unprotect Memory

        pr_info("   Hijacking Syscall Table.\n");
        syscall_table[__NR_shutdown] = (unsigned long) hooked_shutdown;

        my_memory_ro();

        return 0;
}

static void __exit lkm_exit(void)
{
        my_memory_rw();

        pr_info("   Restoring original syscall.\n");        
        syscall_table[__NR_shutdown] = (unsigned long) original_shutdown;

        my_memory_ro();

        pr_info("Exiting... Bye.\n");
}

module_init(lkm_init);           
module_exit(lkm_exit);          

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");

