## Syscall Hooking: getdents and getdents64
---
For Kernel > 4.17.0

### `linux/syscalls.h` -> `getdents` and `getdents64`
---
```c
asmlinkage long sys_getdents(unsigned int fd,
                             struct linux_dirent __user *dirent,
                             unsigned int count);

asmlinkage long sys_getdents64(unsigned int fd,
		               struct linux_dirent64 __user *dirent,
		               unsigned int count);
                      
```
On success, 
- the number of bytes read is returned.  On end of directory, 0 is returned.  On error, -1 is returned.
- and `struct linux_dirent __user *dirent` points to the first `linux_dirent` of the list.

This functions are implemented in `fs/readdir.c`

### `fs/readdir.c` -> `linux_dirent`
---
```c
struct linux_dirent {
	unsigned long     d_ino;            //<- Inode for the dir entry
	unsigned long     d_off;            //<- Offset to next linux_dirent
	unsigned short    d_reclen;         //<- Length for this dirent
	char              d_name[1];        //<- 
};
```
### `linux/dirent.h` -> `linux_dirent64`
---
```c
struct linux_dirent64 {
	u64             d_ino;            //<- Inode for the dir entry
	s64             d_off;            //<- Offset to next linux_dirent
	unsigned short	d_reclen;         //<- Length for this dirent
	unsigned char   d_type;
	char            d_name[0];        //<- File/Dir name NULL terminated
};
```











