#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/slab.h>

#include "my_memory.h"

static unsigned long *__sys_call_table;                             //<- Pointer to the Kernel syscall table

asmlinkage long (*original_chdir) (const struct pt_regs *regs);     //<- Original getdents64 definition

asmlinkage long hooked_chdir (const struct pt_regs *regs)
{
        char __user * file_user = (char *) regs->di;                //<- Pointer to the Path String in User Space
        char * file_kern = NULL;                                    //<- Pointer to the path string in Kernel Space
        int length;                                                 //<- Length of the path string
        int value;                                                  //<- for error checking

        length = strlen(file_user);                                 //<- Path total length

        file_kern = kzalloc(length, GFP_KERNEL);                    //<- Allocate memory to store the path in kernel space
        
        if (file_kern == NULL) {                                    //<- Error Checking
                pr_err("   Error allocating space for path.\n");
                return -EINTR;
        }

        value = copy_from_user(file_kern, file_user, strlen(file_user));   //<- Copy the path from User to Kernel Space

        if (value) {                                                //<- Error Checking
                pr_err("   Error copying from user space.\n");
                return -EINTR;
        }
        
        pr_info("   File is: %s.\n", file_kern);

        kfree(file_kern);                                           //<- Release allocated Kernel Space

        return original_chdir(regs);                                //<- Call to original chdir syscall
        //return 0;

}

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        __sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");     //<- Get address for Kernel Syscall Table

        if (__sys_call_table == NULL) {
                pr_err("  Can't find syscall table!.\n");
                return -1;
        }

        original_chdir = (void *) __sys_call_table[__NR_chdir];                 //<- Save actual address for sys_shutdown

        my_memory_rw();

        __sys_call_table[__NR_chdir] = (unsigned long) hooked_chdir;            //<- Replace the syscall pointer in the syscall table

        my_memory_ro();

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye.\n");

        my_memory_rw();

        __sys_call_table[__NR_chdir] = (unsigned long) original_chdir;          //<- Restore syscall

        my_memory_ro();

}

module_init(lkm_init);           
module_exit(lkm_exit);          

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");

