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
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/cred.h>
#include <linux/sched/signal.h>
#include <linux/path.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/limits.h>
#include <linux/slab.h>
#include <linux/fs.h>

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

/*
 Returns the number of threads in a task group 
*/
static inline int mylkm_get_task_thread_count(struct task_struct *tsk)
{
		return tsk->signal->nr_threads;
}

/*
 Prints the binary name and with the path
 */
static inline int mylkm_print_task_binary_name(struct task_struct *tsk) 
{
	char *temp_path;		//<- Temporary pointer to the path
	char *binary_path;		//<- Real pointer to path string
	
	PTR_NULL_CHECK(tsk);		//<- Check if tsk parameter pointer is not NULL
	
	if (tsk->mm == NULL)		//<- Checks if the Task has a memory map struct defined
	{
		pr_info("Task received is NULL - Reason could be a Kernel Thread\n");
		
	} else {
		
		down_read(&tsk->mm->mmap_sem);		//<- Lock 
		
		if (tsk->mm->exe_file == NULL)		//<- Check if binary file exists
		{
			pr_info("Binary Path: (none)\n");
			
		} else {
						
			temp_path = kmalloc(PATH_MAX, GFP_KERNEL);
			if (temp_path == NULL) panic("Error in kmalloc()\n");
			
			binary_path = d_path(&tsk->mm->exe_file->f_path, temp_path, PATH_MAX);
			
			pr_info("Binary Path: %s\n", binary_path); 
			
			if(NULL != temp_path) kfree(temp_path);
		}
		
		up_read(&tsk->mm->mmap_sem);		//<- Unlock
		
	}

	return DEFAULT_SUCCESS;
}
	
/*
 Print the task's current working directory and root path
 */
static inline int mylkm_print_task_root_path_pwd(struct task_struct *tsk) {

	char *temp_path;
	struct path root_path, pwd_path;
	char *root_path_name, *pwd_path_name;

	PTR_NULL_CHECK(tsk);

	// Root and Pwd for the process

	temp_path = kmalloc(PATH_MAX, GFP_KERNEL);

	if(NULL == temp_path) panic("Error in kmalloc()\n");

	get_fs_root(tsk->fs, &root_path);
	root_path_name = d_path(&root_path, temp_path, PATH_MAX);
	pr_info("Root Path: %s\n", root_path_name);

	get_fs_pwd(tsk->fs, &pwd_path);
	pwd_path_name = d_path(&pwd_path, temp_path, PATH_MAX);
	pr_info("PWD Path: %s\n", pwd_path_name);

        // release memory
        if(NULL != temp_path) kfree(temp_path);

	return DEFAULT_SUCCESS;

}	
	
