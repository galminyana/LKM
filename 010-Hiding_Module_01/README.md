# Stealth 01
---

Loaded modules can be listed by `lsmod` and found through `/proc/modules` and `/sys/modules`.

## Hiding the LKM
--- 
This technique hides the module from `lsmod`, `/proc/modules` and `/sys/modules`. However, won't be able to remove module by using `rmmod`.

### `struct kobject` in `linux/kobject.h`
---
A `kobject` struct represents a kernel object. For each module loaded, exists a `kobject` referencing to it. [More info here](https://www.win.tue.nl/~aeb/linux/lk/lk-13.html)

```c
struct kobject {
	const char *name;
	struct list_head entry;
	struct kobject *parent;
	struct kset *kset;
	struct kobj_type *ktype;
	struct kernfs_node *sd;                 /* sysfs directory entry */
	struct kref	kref;
#ifdef CONFIG_DEBUG_KOBJECT_RELEASE
	struct delayed_work	release;
#endif
	unsigned int state_initialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;
};
```

```c
static void hide_module(void)
{
  list_del_init(&__this_module.list);
  kobject_del(&THIS_MODULE->mkobj.kobj);
}
```





## References
---
- [Kernel Kobject Device Model](https://medium.com/powerof2/the-kernel-kobject-device-model-explained-89d02350fa03)
- [Linux Kernel: Sysfs and Kobjects](https://www.win.tue.nl/~aeb/linux/lk/lk-13.html)
