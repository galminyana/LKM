## Process Info 01
---

Processes information is stored in Internal Data Structures from the Linux Kernel.

In user space, a Process has a Main thread (`main()` function), and depending on the process it spawns Child Threads. In Kernel Space, Kernel mantains a Task List, and inside, the Main Thread and each Child Thread is treated as a Task.

| User Space | Kernel Space |
|-|-|
| ![Alt text](Images/UserSpace.PNG?raw=true "User Space") | ![Alt text](Images/KernelSpace.PNG?raw=true "Kernel Space") |

### User vs Kernel Space
---

| User Space Process | Kernel Space Tasks |
|-|-|
| Only has access to own Virtual Memory | Has access to any memory in Kernel Space |
| Can not access other processes space | Access to everything |
| PID is the Process ID | TGID is the Thread Group ID that identifies Main Thead |
| Thread ID identifies threads | PID identifies Task ID. Main Thread TGID == PID and for Other Threads TGID != PID |
| Easy to dump mempry | Need to lock memory before access to it |

#### TGID and PID Relationship

In Kernel, for each thread of a process, the TGID and PID will be stored. For the Main thread, the TGID and PID will be the same, but for each of the Child Threads, the TGID will be the same as the Main Thread, but the PID will be different.

| TGID vs PID |
|-|
| ![Alt text](Images/TGIDandPID.png?raw=true "TGID and PID Relationship") |

### Struct `task_struct` at `include/linux/sched.h`
---
Each task in the Kernel is represented by this structure
```c
struct task_struct
```
The relevant members of this structure are:

- `pid_t pid;` and `pid_t tgid;`: To store the PID and TGID
- `char comm[TASK_COM_LEN];`: Stores the executable name without the full path
- `struct task_struct __rcu *real_parent;`:
- `struct task_struct __rcu *parent;`: 

### Kernel Functions for Processes
---
#### `find_vpid()` - Find PID
Finds the PID by it's virtual id. 
```c
extern struct pid *find_vpid(int nr);
```
Returns a pointer to a `pid` structure
```c 
struct pid
{
	atomic_t count;
	unsigned int level;
	struct hlist_head tasks[PIDTYPE_MAX];   	/* lists of tasks that use this pid */
	struct rcu_head rcu;
	struct upid numbers[1];
};
```
#### `pid_task` - Get `task_struct`
From a `pid` struct as parameter, returns a pointer to `task_struct`:
```c
extern struct task_struct *pid_task(struct pid *pid, enum pid_type);
```
### Code
---
#### proc_info
```c

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
MODULE_DESCRIPTION("LKM Processes Example");
```
#### my_lkm_lib.h
```c
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
```
### Results
---
```bash
LKM# ps ax
  823 ?        Sl     0:00 lightdm --session-child 14 23
LKM# insmod proc_info.ko process_id=823
LKM# dmesg
[ 3876.225134] proc_info: unknown parameter 'process' ignored
[ 3876.225237] proc_info->lkm_init:29:  Invalid PID: 0
[ 3895.328834] proc_info->lkm_init:18:  ======= Process Details ========
[ 3895.328839] proc_info->lkm_init:19:  PID to check: 823
[ 3895.328841] proc_info->mylkm_print_task_pid_details:57:  Task Binary: lightdm TGID: 823 PID: 823
[ 3895.328845] proc_info->mylkm_print_task_parent_pid_details:71:  Parent Task Binary: lightdm TGID: 724 PID: 724
[ 3895.328880] proc_info->lkm_init:25:  ================================
LKM# rmmod proc_info
LKM# dmesg -C
LKM# insmod proc_info.ko process_id=724
LKM# dmesg
[ 3992.577608] proc_info->lkm_init:18:  ======= Process Details ========
[ 3992.577612] proc_info->lkm_init:19:  PID to check: 724
[ 3992.577615] proc_info->mylkm_print_task_pid_details:57:  Task Binary: lightdm TGID: 724 PID: 724
[ 3992.577618] proc_info->mylkm_print_task_parent_pid_details:71:  Parent Task Binary: systemd TGID: 1 PID: 1
[ 3992.577619] proc_info->lkm_init:25:  ================================
LKM# rmmod proc_info
LKM# dmesg -C
LKM# insmod proc_info.ko process_id=1
LKM# dmesg
[ 4047.321423] proc_info->lkm_init:18:  ======= Process Details ========
[ 4047.321427] proc_info->lkm_init:19:  PID to check: 1
[ 4047.321430] proc_info->mylkm_print_task_pid_details:57:  Task Binary: systemd TGID: 1 PID: 1
[ 4047.321432] proc_info->mylkm_print_task_parent_pid_details:71:  Parent Task Binary: swapper/0 TGID: 0 PID: 0
[ 4047.321434] proc_info->lkm_init:25:  ================================
LKM# rmmod proc_info
LKM# dmesg -C
LKM# insmod proc_info.ko process_id=0
insmod: ERROR: could not insert module proc_info.ko: Invalid parameters
LKM# dmesg
[ 4096.633774] proc_info->lkm_init:29:  Invalid PID: 0
LKM#
```















