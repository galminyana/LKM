## Process Info 01
---

LEt's explore Internal Data Structures in the Kernel, where information for Processes is stored.

In user space a Process has a Main thread (`main()` function), and depending on the process it spawns Child Threads. In Kernel Space, Kernel mantains a Task List, and inside, the Main Thread and each Child Thread is treated as a Task.

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

- `comm`: 
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
















