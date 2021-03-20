## Mutex
---

### `linux/mutex.h`
---
Only one task can hold the mutex. The owner of the mutex is the task
```c
struct mutex {
	atomic_long_t		owner;
	spinlock_t		wait_lock;
#ifdef CONFIG_MUTEX_SPIN_ON_OWNER
	struct optimistic_spin_queue osq; /* Spinner MCS lock */
#endif
	struct list_head	wait_list;
#ifdef CONFIG_DEBUG_MUTEXES
	void			*magic;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};
```
### Mutex Initialization
---
#### Static
```c
static DEFINE_MUTEX(my_mutex);
```
### Dinamic
Declare a mutex, and initialize it using
```c
mutex_init(struct mutex *lock);
```
Example:
```c
struct mutex my_mutex;
mutex_init(&my_mutex);
```
### Mutex Lock
---
- ```**mutex_lock```Aquire the mutex, and if already locked, sleep until available
```c
void mutex_lock(struct mutex *lock);
```
- 



### References
---
- [Bootlin](https://elixir.bootlin.com/linux/v4.17.18/source/include/linux/mutex.h#L53)
