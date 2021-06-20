#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>

#include "my_memory.h"


static unsigned long *__sys_call_table;                          //<- Pointer to the Kernel syscall table

asmlinkage int (*original_kill) (const struct pt_regs *regs);

/*
 For regs, params are:
 - si: Signal
 - di: Process_id
*/
asmlinkage int hooked_kill(const struct pt_regs *regs)
{
        pr_info("   Hooked Syscall!!\n");

        pr_info("   Signal %d for process %d.\n", (int) regs->si, (int) regs->di);

        //return original_kill(regs);
        return 0;
}


static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        __sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");     //<- Get address for Kernel Syscall Table

        if (__sys_call_table == NULL) {                                         //<- Error Checking
                pr_err("  Can't find syscall table!.\n");
                return -EINTR;
        }

        original_kill = (void *) __sys_call_table[__NR_kill];                   //<- Save actual address for sys_kill

        my_memory_rw();                                                         //<- Unprotect Memory

        __sys_call_table[__NR_kill] = (unsigned long) hooked_kill;              //<- Hook the Syscall Table with the
                                                                                //   address of hooking function
        my_memory_ro();

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye.\n");

        my_memory_rw();

        __sys_call_table[__NR_kill] = (unsigned long) original_kill;             //<- Restore original Syscall

        my_memory_ro();

}

module_init(lkm_init);           //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");

