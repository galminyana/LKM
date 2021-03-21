#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/kallsyms.h>

static unsigned long my_cr0;                            //<- Shadow for CR0 register
asmlinkage long (*orig_sys_shutdown)(int, int);         //<- Pointer to store the original sys_shutdown syscall
                                                        //   Value is saved during hooking
static unsigned long *syscall_table;                    //<- @ for System Call Kernel Table

static inline void my_write_cr0(unsigned long val)
{
        unsigned long __force_order;
        asm volatile("mov %0, %%cr0" : "+r"(val), "+m"(__force_order));
}

/*
 Memory Protected (RO)
*/
static inline void my_memory_ro(void)
{
        my_write_cr0(my_cr0);
}

/*
 Memory Unprotected (RW)
*/
static inline void my_memory_rw(void)
{
        my_write_cr0(my_cr0 & ~0x00010000);
}

asmlinkage int hooked_shutdown(int param1, int param2)
{
        pr_info("   Hooked Syscall!!\n");
        //return orig_shutdown(param1, param2);                 //<- Jums to run original syscall
        return 0;
}

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");


        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye.\n");
}

module_init(lkm_init);           //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");
