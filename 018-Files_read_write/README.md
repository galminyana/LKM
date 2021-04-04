## User Space Files
---
For Kernel > 4.17.0

### Open File
---
```c 
/**
 * filp_open - open file and return file pointer
 *
 * @filename:	path to open
 * @flags:	open flags as per the open(2) second argument
 * @mode:	mode for the new file if O_CREAT is set, else ignored
 *
 * This is the helper to open a file from kernelspace if you really
 * have to.  But in generally you should not do this, so please move
 * along, nothing to see here..
 */
struct file *filp_open(const char * filename, int flags, umode_t mode);
```
Where `flags` can be:
```c
#define O_ACCMODE	00000003
#define O_RDONLY	00000000       //<- For reading
#define O_WRONLY	00000001       //<- For writing
#define O_RDWR		00000002       //<- Both ;-)
#define O_CREAT		00000100	     //<- Creates new file. Overwrites if exist
```
`mode` just required if `flag` is `O_CREAT`, otherwise ignored and can be set to `0`. It's to indicate the file permissions, like `0777` for read/write/execute for everyone, or 0700 for full permissions for root.

### Close File
---
```c 
int filp_close(struct file *filp, fl_owner_t id)
```
- `filp` comes from the `file` struct returned on `filp_open`.
- `id` can be set to current

### Read and Write File
---
```c 
ssize_t vfs_read  (struct file * filp, char __user * buffer, size_t len, loff_t * pos);
ssize_t vfs_write (struct file * filp, const char __user * buffer, size_t len, loff_t * pos);
```
On both functions, as the `buffer` parameter has the `__user` keyword, both pointers have to point to User Space. 

### User Space to `buffer`
---
To make `vfs_read` and `vfs_write` functions to use the buffer pointer in Kernel Space, need to use the `set_fs()` function:
```c
void set_fs(mm_segment_t fs);
```
Values for `fs`:
- USER_DS : Kernel default value. Transforms to User Space addresses
- KERNEL_DS : Transforms to Kernel Space addresses.

To get the actual status of addresses, use the `get_fs()` function:
```c
mm_segment_t get_fs();
```
#### Declaration of `get_fs()` and `set_fs()` macros
```c
#define get_fs()  (current_thread_info()->addr_limit)
#define set_fs(x) (current_thread_info()->addr_limit = (x))
```




### References
---

- [Reading and Writing of Files in Linux Kernel Driver](https://www.programmersought.com/article/83015124510/)

