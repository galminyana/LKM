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
This functions are implemented in `fs/readdir.c`

### `fs/readdir.c` -> `linux_dirent`
---
```c
struct linux_dirent {
	unsigned long	  d_ino;
	unsigned long	  d_off;
	unsigned short	d_reclen;
	char		        d_name[1];
};
```
### `linux/diret.h` -> `linux_dirent64`
---
```c
struct linux_dirent64 {
	u64		          d_ino;
	s64		          d_off;
	unsigned short	d_reclen;
	unsigned char	  d_type;
	char		        d_name[0];
};
```











