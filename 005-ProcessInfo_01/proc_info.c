
#include "mylkmlib.h"			//<- Own lib
#include <linux/module.h>
#include <linux/init.h>


/* Module parameter definition */
static int process_id;
module_param(process_id, int, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(process_id ,"Process ID for the target process");

static int __init lkm_init(void)
{
	struct task_struct *target_process = target_process = mylkm_get_task_struct_by_pid(process_id);  //<- target_process := Process information
	struct task_struct *process_thread;
	
	if(!IS_ERR_OR_NULL(target_process))      //<- Check if got an error from previous call
	{
		pr_info("======= Process Details ========\n");
		pr_info("PID to check: %d\n", process_id);
		
		mylkm_print_task_pid_details(target_process);  //<- Prints Name, TGID, PID
		
		mylkm_print_task_parent_pid_details(target_process);  //<- Prints the same but for parent
		
		pr_info("================================\n");
		
	} else {			//<- Error
		
		pr_info("Invalid PID: %d\n", process_id);
		return -EINVAL;
	}	
	
	return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("LKM Example");
