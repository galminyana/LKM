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
struct mutex my_mutex;		//<- Declaration
mutex_init(&my_mutex);		//<- Initialize
```
### Mutex Lock
---
- `mutex_lock`: Acquire the mutex, and if already locked, sleep until available.
```c
void mutex_lock(struct mutex *lock);
```
- `mutex_lock_interruptible`: Locks the mutex and returns 0 if the mutex has been acquired. If mutex not available, sleeps until the mutex becomes available. If signal arrives when sleeping then this function returns -EINTR.
```c
int mutex_lock_interruptible(struct mutex *lock);
```
- `mutex_trylock`: Tries to acquire the mutex without waiting (does not sleep). Returns 1 if the mutex has been acquired successfully, and 0 if mutex is not available.
```c 
int mutex_trylock(struct mutex *lock);
```

### Mutex Unlock
---
```c
void mutex_unlock(struct mutex *lock);
```

### Mutex Info
---
- To check if a mutex is locked.
```c 
int mutex_is_locked(struct mutex *lock);
```
### Mutex Destroy
---
```c 
mutex_destroy(&my_char_mutex);
```

