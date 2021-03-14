## Character Device
---


### `linux/fs.h` -> `struct file_operations`
--- 
The file_operations structure from `linux/fs.h` lists the callback functions associated with file operations. Char devices usually implement open, read, write and release calls.
```c
struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	...[REMOVED]...
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *, fl_owner_t id);
	int (*release) (struct inode *, struct file *);
	...[REMOVED]...
} __randomize_layout;
```
Functions required once implemented, have to be assigned to the struct


- OPENCallback function called each time the device is opened. 
 - @pinode A pointer to an inode object (defined in linux/fs.h)
 - @pfile A pointer to a file object (defined in linux/fs.h)
```c
static int dev_open(struct inode *pinode, struct file *pfile) 
{
    pr_info("Device opened\n");
    return 0;
}
```
/* From device to user, like a cat /dev/device */
```c
ssize_t dev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{
    pr_info("User Reading from char device\n");
    return 0;
}
```
/* From user to device, like a echo 1 > /dev/device */
```c 
ssize_t dev_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
    pr_info("User Writing to device\n");
    return 0;
}
```

```c 
static int dev_close(struct inode *pinode, struct file *pfile)
{
    pr_info("Device closed\n");
    return 0;
}
```
The callback functions are defined in the struct `file_operations`:
```c 
struct file_operations dev_file_operations = { 
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_close,
};
```

### Register / Unregister Device
---
To do use the functions:
```c
/*
 If major == 0 this functions will dynamically allocate a major and return its number.
 If major > 0 this function will attempt to reserve a device with the given major number and will return zero on success.
 Returns a -ve errno on failure.
*/
static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops);

static inline void unregister_chrdev(unsigned int major, const char *name);
```

