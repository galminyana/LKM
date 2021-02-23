## Process Info 02 (Threads)
---

### More on `task_struct`
---

On Signal Handlers Section, can find:
```c
struct signal_struct *signal;
```

Also:
```c
struct list_head thread_group;
struct list_head thread_node;
```
- `thread_node`: 
- `thread_group`: 

### `linux/sched/signal.h` -> struct `signal_struct` 
---
This struct defines:

```c
int nr_threads;			//<- This keeps the number of threads in the thread group
struct list_head thread_head;	//<- List with threads
```

### `linux/types.h` -> struct `list_head`
---
```c
struct list_head {
	struct list_head *next, *prev;
};
```
### Macros of interest on `linux/sched/signal.h`
---
```c
/*
 * Careful: do_each_thread/while_each_thread is a double loop so
 *          'break' will not work as expected - use goto instead.
 */
#define do_each_thread(g, t) \
	for (g = t = &init_task ; (g = t = next_task(g)) != &init_task ; ) do

#define while_each_thread(g, t) \
	while ((t = next_thread(t)) != g)

#define __for_each_thread(signal, t)	\
	list_for_each_entry_rcu(t, &(signal)->thread_head, thread_node)

#define for_each_thread(p, t)		\
	__for_each_thread((p)->signal, t)
```

