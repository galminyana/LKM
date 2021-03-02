## Process Info 03 (Bin executable and Paths)
---

### More on `task_struct`
---
The struct, defines a:
```c
char comm[TASK_COMM_LEN];		//<- Name of the binary for the task
```
Where `TASK_COMM_LEN` is defined as:
```c
#define TASK_COMM_LEN 16
```
That means that the executable name is trucated at 16 bytes. **But not the full path**

### Let's go for the full path
---
In the `task_struct`, also has defined a 
```c
struct mm_struct *mm;			//<- Task Memory Map
```
If this struct pointer value is NULL, can be because the task is a thread.

### `linux/mm_types.h` -> `struct mm_struct`
---
This struct, defines a
```c
struct file __rcu *exe_file;		//<- Struct to store information about the binary file
```
If this struct pointer value is NULL, can be because the task does not have a path

Also defines 
```c
struct rw_semaphore mmap_sem;
```
This is the semaphore to lock/unlock the memory map. To do, can use the following functions defined at `linux/rwsem.h`:
```c
extern void down_read(struct rw_semaphore *sem);        //<- Hold memory map semaphore to read (locks fpr reading)
extern void up_read(struct rw_semaphore *sem);          //<- Releases memory map semaphore from reading (unlocks from reading)
extern void down_write(struct rw_semaphore *sem);       //<- Locks for writing
extern void up_write(struct rw_semaphore *sem);         //<- Unlocks from writing
```
It's mandatory to lock/unlock when accessing the memory map struct.
### `linux/fs.h` -> `struct_file`
---
Defines a 
```c
struct path f_path;			//<- Struct to store full binary path
```
To get the path string, can use the following kernel function defined at `linux/fs/d_path.c`, that returns a string:
```c
/**
 * d_path - return the path of a dentry
 * @path: path to report
 * @buf: buffer to return value in
 * @buflen: buffer length
 *
 * Convert a dentry into an ASCII path name. If the entry has been deleted
 * the string " (deleted)" is appended. Note that this is ambiguous.
 *
 * Returns a pointer into the buffer or an error code if the path was
 * too long. Note: Callers should use the returned pointer, not the passed
 * in buffer, to use the name! The implementation often starts at an offset
 * into the buffer, and may leave 0 bytes at the start.
 *
 * "buflen" should be positive.
 */
char *d_path(const struct path *path, char *buf, int buflen);
```
### `linux/path.h` -> `struct path`
---
```c 
struct path {
	struct vfsmount *mnt;
	struct dentry *dentry;
} __randomize_layout;
```
To access the path, Kernel provides functions and macros.
### `linux/fs_struct.h` -> Functions and Macros to get the Path, Root and PWD
---
```c
/**
 * Returns the root
 * @fs: For example tsk->fs
 * @root: buffer to return value in
 */
static inline void get_fs_root(struct fs_struct *fs, struct path *root);
/**
 * Returns the PWD
 * @fs: For example tsk->fs 
 * @pwd: buffer to return value in
 */
static inline void get_fs_pwd(struct fs_struct *fs, struct path *pwd);
```
### `linux/slab.h`
---
The `malloc()` and `free()` equivalents on Kernel are defined:
```c
/**
 * kmalloc - allocate memory
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate.
 */
static __always_inline void *kmalloc(size_t size, gfp_t flags);
/**
 * kfree - free previously allocated memory
 * @objp: pointer returned by kmalloc.
 * If @objp is NULL, no operation is performed.
 */
void kfree(const void *objp);
```
