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




