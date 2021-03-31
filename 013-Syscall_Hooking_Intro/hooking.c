#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>

#include "my_memory.h"

static unsigned long *__sys_call_table;					//<- Pointer to the Kernel syscall table

//asmlinkage int (*original_shutdown) (int, int);
asmlinkage int (*original_mkdir) (const char * path);

asmlinkage int hooked_mkdir(const char * path)
{
	pr_info("   Hooked Syscall!!\n");
	//return orig_shutdown(i, z);
	return 0;
}

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

  	__sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");  //<- Get address for Kernel Syscall Table
 
  	if (__sys_call_table == NULL) {
		pr_err("  Can't find syscall table!.\n");
    		return -1;
  	}

	original_mkdir = (void *) __sys_call_table[__NR_mkdir];	            //<- Save actual address for sys_shutdown

	my_memory_rw();					                    //<- Unprotect Memory

	__sys_call_table[__NR_mkdir] = (unsigned long) hooked_mkdir;        //<- Replace the syscall table entry for the
	                                                                    //   syscall with out hooked function
	my_memory_ro();                                                     //<- Protect memory

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye.\n");

	my_memory_rw();					                    //<- Unprotect Memory

	__sys_call_table[__NR_mkdir] = (unsigned long) original_mkdir;      //<- Restore Syscall Table with original syscall

	my_memory_ro();                                                     //<- Protect memory

}

module_init(lkm_init);           
module_exit(lkm_exit);          

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Hook Syscall Example");
