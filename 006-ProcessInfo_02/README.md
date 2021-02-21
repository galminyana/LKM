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

### `linux/sched/signal.h` -> `signal_struct` 
---

This struct defines:

```c
int nr_threads;
```
This keeps the number of threads in the thread group.

### `list_head`
---
```c
struct list_head {
	struct list_head *next, *prev;
};
```


