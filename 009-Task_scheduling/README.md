## Task Scheduler
---

For Kernel > 4.17.0

Can define a function to call on every timer interrupt.

### Steps
---

1) Create a task in a `workqueue_struct` structure that will hold a pointer to the function to call
2) Use the function `queue_Delayed_work()` to put this task in the tasks list
3) Once called, have to put the task back again on the queue to be called again

Once the module has to be unloaded, must take care to remove the function from the task list and wait until next execution

### `workqueue.c` -> workqueue_Struct`
---
Where to create the task list

### `linux/workqueue.h` -> `work_struct`
---
```c
struct work_struct {
	atomic_long_t data;
	struct list_head entry;
	work_func_t func;                 //<- Our function
#ifdef CONFIG_LOCKDEP
	struct lockdep_map lockdep_map;
#endif
};
```

### `DECLARE_WORK` Macro
---
To initialize and assign the function to the `work_struct` task

```c
#define DECLARE_WORK(n, f)						\
	struct work_struct n = __WORK_INITIALIZER(n, f)
```