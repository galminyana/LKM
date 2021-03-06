/*
 Author: Guillem Alminyana
 License: GPV v3

 Library with usefull functions to deal with Processes in Linux Kernel Space

*/


/*
 Let's define formatting for log printing.
 Adds the fuction name and the line of the code where log is printed
 */
#define pr_fmt(fmt)	KBUILD_MODNAME "->%s:%d:  " fmt, __func__,__LINE__

/*********************/
/* REQUIRED INCLUDES */
/*********************/
#include<linux/sched.h>		//<- Defines the 'task_struct'
#include<linux/pid.h>
#include<linux/cred.h>

/*
 Constants definition
*/
#define DEFAULT_SUCCESS	1
#define DEFAULT_FAILURE -1

/**********/
/* MACROS */
/**********/

/* If the parameter is NULL, returns -1 */
#define PTR_NULL_CHECK(ptr)	if(NULL == ptr) return DEFAULT_FAILURE;

/*************/
/* FUNCTIONS */
/*************/

/*
 Receives a PID and returns a task_struct with the process information
*/
static inline struct task_struct * mylkm_get_task_struct_by_pid(int pid) 
{
	return pid_task(find_vpid(pid), PIDTYPE_PID);	
}

/*
 Prints log info about a task_struct.
*/
static inline int mylkm_print_task_pid_details(struct task_struct *tsk) 
{
	PTR_NULL_CHECK(tsk);		//<- Check if tsk parameter pointer is not NULL
	pr_info("Task Binary: %s TGID: %d PID: %d\n", 
			tsk->comm,
			tsk->tgid,		
			tsk->pid);
	return DEFAULT_SUCCESS;
}

/*
 Prints log info about the parent process for the process. Receives the 'task_Struct' for the process
*/
static inline int mylkm_print_task_parent_pid_details(struct task_struct *tsk) 
{
	PTR_NULL_CHECK(tsk);		//<- Check if tsk parameter pointer is not NULL
	/* Process 'task_struct' holds a pointer to the parent 'task_struct' or NULL if is the init() process */
	pr_info("Parent Task Binary: %s TGID: %d PID: %d\n",
			tsk->real_parent->comm,
			tsk->real_parent->tgid,
			tsk->real_parent->pid);
	return DEFAULT_SUCCESS;
}



