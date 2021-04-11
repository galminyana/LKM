## Task Scheduler
---

For Kernel > 4.17.0

Can define a function to call on every timer interrupt.

### Steps
---

1) Create a task in a `workqueue_struct` structure that will hold a pointer to the function to call
2) Use the function `queue_delayed_work()` to put this task in the tasks list
3) Once called, have to put the task back again on the queue to be called again

Once the module has to be unloaded, must take care to remove the function from the task list and wait until next execution

### `DECLARE_WORK` Macro
---
The macro is defined on `linux/workqueue.h`, and it creates a `work_struct`:

```c
#define DECLARE_WORK(n, f)						\
	struct work_struct n = __WORK_INITIALIZER(n, f)
```
Where:
- n (name): Is the name of the `work_struct` structure that has to be created.
- f (func): Is the function to be scheduled in this workqueue.
The definition of the parameters:
```c
DECLARE_WORK(name, void (*func)(void *))
```

### `workqueue.c` -> `workqueue_struct`
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

### Schedule Tasks
---
```c
/**
 * schedule_work - put work task in global workqueue
 * @work: job to be done
 *
 * Returns %false if @work was already on the kernel-global workqueue and
 * %true otherwise.
 *
 * This puts a job in the kernel-global workqueue if it was not already
 * queued and leaves it in the same position on the kernel-global
 * workqueue otherwise.
 *
 * Shares the same memory-ordering properties of queue_work(), cf. the
 * DocBook header of queue_work().
 */
static inline bool schedule_work(struct work_struct *work);
```
Returns "0" if the `work` is already on the Kernel Work Queue and non-zero otherwise. 
