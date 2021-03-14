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


- **OPEN_Callback**: function called each time the device is opened. 
  - **@pinode**: A pointer to an inode object (defined in linux/fs.h)
  - **@pfile**: A pointer to a file object (defined in linux/fs.h)
```c
static int dev_open(struct inode *pinode, struct file *pfile) 
{
    pr_info("Device opened\n");
    return 0;
}
```
- **READ_Callback**: This function is called whenever device is being read from user space
  - **@pfile**: A pointer to a file object (defined in linux/fs.h)
  - **@buffer**: The pointer to the buffer to which this function writes the data
  - **@length**: Buffer Length
  - **@offset**: The offset (if required)

```c
ssize_t dev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{
    pr_info("User Reading from char device\n");
    return 0;
}
```
- **WRITE_Callback**: This function is called when the device is being written to from user space
  - **@pfile**: A pointer to a file object (defined in linux/fs.h)
  - **@buffer**: The buffer to that contains the string to write to the device
  - **@length**: Buffer Length
  - **@offset**: The offset (if required)
```c 
ssize_t dev_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
    pr_info("User Writing to device\n");
    return 0;
}
```
- **RELEASE_Callback**: Release function that is called when the device is closed/released by the userspace program
  - **@pinode**: A pointer to an inode object (defined in linux/fs.h)
  - **@pfile**: A pointer to a file object (defined in linux/fs.h)
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
 Returns a -ve errno on failure. If  major is 0, returns the major number if no error
*/
static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops);

static inline void unregister_chrdev(unsigned int major, const char *name);
```

### Create / Destroy the Device Class
---
Have to define a `class` pointer:
```c
struct class {
	const char		*name;
	struct module		*owner;

	const struct attribute_group	**class_groups;
	const struct attribute_group	**dev_groups;
	struct kobject			*dev_kobj;

	int (*dev_uevent)(struct device *dev, struct kobj_uevent_env *env);
	char *(*devnode)(struct device *dev, umode_t *mode);

	void (*class_release)(struct class *class);
	void (*dev_release)(struct device *dev);

	int (*shutdown_pre)(struct device *dev);

	const struct kobj_ns_type_operations *ns_type;
	const void *(*namespace)(struct device *dev);

	const struct dev_pm_ops *pm;

	struct subsys_private *p;
};
```
And then register the device class using the macro, where owner is the module (`THIS_MODULE`) and the name the string for the class name
```c 
#define class_create(owner, name)	
```
Example:
```c 
#define DEVICE_CLASS "name"
#define DEVICE_NAME  "file_name"

static struct class*  device_class =  NULL;

device_class = class_create(THIS_MODULE, DEVICE_CLASS);   
```
To unregister a Device Class, use the function below, knowing that the parameter is the device class returned when created before:
```c 
extern void class_destroy(struct class *cls);
```
Example:
```c
class_destroy(device_class); 
```
### Create / Destroy the Device Driver
---
A device driver object is defined on a `device_struct` defined at `linux/device.h`.
To Create a device driver, have to declare a pointer to a device_struct:
```c
static struct device* device_struct = NULL; 
```
And call the function:
```c
struct device *device_create(struct class *cls, struct device *parent, dev_t devt, void *drvdata, const char *fmt, ...);
```
Example:
```c 
#define DEVICE_CLASS "my_char_device"
#define DEVICE_NAME  "my_char"

static struct device* device_struct = NULL;  

device_struct = device_create(device_class,NULL, MKDEV(number_major, 0), NULL, DEVICE_NAME);  
```
The `MKDEV` macro creates the `mknod` file with the major and minur numbers indicated, and is defined as:
```c
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))
```
To Delete a Device Driver, use the function:
```c 
extern void device_destroy(struct class *cls, dev_t devt);
```
Example:
```c 
device_destroy(device_class, MKDEV(number_major, 0));
```




### References
---
