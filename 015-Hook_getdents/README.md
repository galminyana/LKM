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
Parameters are a file descriptor, a `linux_dirent` struct that will be updated after the syscall, and the count.
On success, 
- the number of bytes read is returned.  On end of directory, 0 is returned.  On error, -1 is returned.
- and `struct linux_dirent __user *dirent` points to the first `linux_dirent` of the list of direntries.

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






### `kmalloc` vs `kzalloc`
---
`kmalloc` does not initialize memory. `kzalloc` sets memory to zero.
```c
/**
 * kmalloc - allocate memory
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate.
 */
static __always_inline void *kmalloc(size_t size, gfp_t flags)
{
	if (__builtin_constant_p(size)) {
		if (size > KMALLOC_MAX_CACHE_SIZE)
			return kmalloc_large(size, flags);
#ifndef CONFIG_SLOB
		if (!(flags & GFP_DMA)) {
			unsigned int index = kmalloc_index(size);

			if (!index)
				return ZERO_SIZE_PTR;

			return kmem_cache_alloc_trace(kmalloc_caches[index],
					flags, size);
		}
#endif
	}
	return __kmalloc(size, flags);
}

/**
 * kzalloc - allocate memory. The memory is set to zero.
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate (see kmalloc).
 */
static inline void *kzalloc(size_t size, gfp_t flags)
{
	return kmalloc(size, flags | __GFP_ZERO);
}
```








