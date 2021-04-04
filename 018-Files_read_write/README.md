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

### References
---

- [Reading and Writing of Files in Linux Kernel Driver](https://www.programmersought.com/article/83015124510/)

