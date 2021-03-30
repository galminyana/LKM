#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/slab.h>

#include "my_memory.h"

static unsigned long *__sys_call_table;                             //<- Pointer to the Kernel syscall table

asmlinkage long (*original_getdents64) (const struct pt_regs *regs);//<- Original getdents64 syscall definition

asmlinkage int hooked_getdents64(const struct pt_regs *regs)
{
        struct linux_dirent64 __user * dirent_user =                //<- dirent_user gets the pointer from RSI register
                               (struct linux_dirent64 *)regs->si;
        struct linux_dirent64 * dirent_ker = NULL;                  //<- The dirent in the kernel space
        struct linux_dirent64 * dirent_current = NULL;              //<- Will point to the actual dirent
        long values;                                                //<- Will store results for error checking
        unsigned long offset = 0;                                   //<- Offset used to point to the next linux_dirent64
        int size = 0;                                               //<- Stores the size of the dirent entries

        size = original_getdents64(regs);                           //<- Call the original getdents64 to get the linux_direntry-es
        if (size <= 0) {
                pr_err("   Error calling original getdents64.\n");
                return size;
        }

        dirent_ker = kzalloc(size, GFP_KERNEL);                     //<- Reserve memory to copy the returned linux_direntry-es from previous line
        if (dirent_ker == NULL) {                                   //<- Error checking.
                pr_err("   Error allocating memory space.\n");
                return size;
        }

        values = copy_from_user(dirent_ker, dirent_user, size);     //<- Move from user space to kernel space
        if (values) {                                               //   dirent_kern will be a copy of the dirent_user
                pr_err("   Error copying from user to kernel space.\n");
                return size;
        }

        while (offset < size)                                       //<- Loop over all direntries. When offset reaches
        {                                                           //   the total size, we are done
                dirent_current = (void *)dirent_ker + offset;       //<- Current directory pints to next one. 
                                                                    //   dirent_ker plus offset, points the right mem position
                pr_info("   ENTRY: %s, size %d.\n", dirent_current->d_name, dirent_current->d_reclen);
                offset += dirent_current->d_reclen;                 //<- Update the offset for the next dirent
        }

        values = copy_to_user(dirent_user, dirent_ker, size);       //<- Return to user space the dirent in kernel space
        if (values) {
                pr_err("    Error copying from kernel to user space.\n");
                return size;
        }

        kfree(dirent_ker);                                          //<- Release memory

        return size;
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

