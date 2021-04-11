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

### `DECLARE_WORK` and `DECLARE_DELAYED_WORK` Macros
---
The macro is defined on `linux/workqueue.h`, and it creates a `work_struct`:

```c
#define DECLARE_WORK(n, f)						\
	struct work_struct n = __WORK_INITIALIZER(n, f)
```
```c
#define DECLARE_DELAYED_WORK(n, f)					\
	struct delayed_work n = __DELAYED_WORK_INITIALIZER(n, f, 0)
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
1) Create a Work Queue
2) Queue the task in the Work Queue
3) When task run, remember to Queue it again if want to execute again
4) On module unload, cancel the work, flush, and destroy the Work Queue

#### `create_workqueue`

#### `queue_delayed_work`

```c
/**
 * queue_delayed_work - queue work on a workqueue after delay
 * @wq: workqueue to use
 * @dwork: delayable work to queue
 * @delay: number of jiffies to wait before queueing
 *
 * Equivalent to queue_delayed_work_on() but tries to use the local CPU.
 */
static inline bool queue_delayed_work(struct workqueue_struct *wq,
				      struct delayed_work *dwork,
				      unsigned long delay)
```	

#### `cancel_delayed_work`

#### `flush_delayed_work`

#### `destroy_workqueue`

