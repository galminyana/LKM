## Character Device. Read and Write
---

### Read and Write from/to Userspace
---
```c
static __always_inline unsigned long __must_check copy_to_user(void __user *to, const void *from, unsigned long n);
```
```c
static __always_inline unsigned long __must_check copy_from_user(void *to, const void __user *from, unsigned long n)
```

### Mutex (`linux/mutex.h`)
---
Linux provides semaphores for mutex locks. A semaphore is defined as a struct:
```c
/* Please don't access any members of this structure directly */
struct semaphore {
	raw_spinlock_t		lock;
	unsigned int		count;
	struct list_head	wait_list;
};
```
To define the semaphore in the code, use the `DEFINE_SEMAPHORE` macro.
```c 
#define DEFINE_MUTEX(name);
```
The semaphore must be initialized using the `mutex_init` macro, and the param is the defined one before.
```c 
#define mutex_init(mutex)
```
To lock the semaphore use `mutex_try_lock` that will return 1 or 0 if the mutex been successfully locked. Error checking is required.
```c
/* Returns 1 if the mutex has been acquired successfully, and 0 on contention. */
extern int mutex_trylock(struct mutex *lock);
```
To finish, the semaphore has to be destroyed.
```c
extern void mutex_destroy(struct mutex *lock);
```


