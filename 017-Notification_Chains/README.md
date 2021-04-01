## Notification Chains
---
For Kernel > 4.17.0

Mechanism to allow communication between different Kernel subsystems.

### Related Structures and Functions (`linux/notifier.h`)
---

### `notifier_block`

```c
struct notifier_block {
	notifier_fn_t notifier_call;           //<- Pointer to Callback function
	struct notifier_block __rcu *next;     //<- Next notifier block of the chain
	int priority;                          //<- Priority of this block in the chain
};
```
Each Kernel subsystem that wants to be notified of an event, must provide it's own `notifier_fn_t` callback function in the `notifier_call` entry of the struct

### `notifier_call`: Callback Function

```c
typedef int (*notifier_fn_t)(struct notifier_block *nb, 
                             unsigned long action, 
                             void *data);
```
Parameters needed are:
- `nb`: pointer to the first notification block of the chain 
- `action`: value indicated of the event occurred
- `data`: pointer to pass extra information for the event
And returns:
```c
#define NOTIFY_DONE	  0x0000		              /* Don't care */
#define NOTIFY_OK         0x0001		              /* Suits me */
#define NOTIFY_STOP_MASK  0x8000		              /* Don't call further */
#define NOTIFY_BAD        (NOTIFY_STOP_MASK|0x0002)           /* Bad/Veto action */
#define NOTIFY_STOP       (NOTIFY_OK|NOTIFY_STOP_MASK)
```
Where:
- NOTIFY_DONE:  notified subsystem not interested in this event.
- NOTIFY_OK: notification was processed correctly.
- NOTIFY_BAD: some error, don't call callback functions for this event
- NOTIFY_STOP: notification processed OK, but don't call more callbacks for this event.
- NOTIFY_STOP_MASK: applied to make other stop

### Notification Chains Types
Four Types:
- Blocking notifier chains
- SRCU notifier chains
- Atomic notifier chains
- Raw notifier chains

```c
struct atomic_notifier_head {
	spinlock_t lock;
	struct notifier_block __rcu *head;
};

struct blocking_notifier_head {
	struct rw_semaphore rwsem;
	struct notifier_block __rcu *head;
};

struct raw_notifier_head {
	struct notifier_block __rcu *head;
};

struct srcu_notifier_head {
	struct mutex mutex;
	struct srcu_struct srcu;
	struct notifier_block __rcu *head;
};
```

### Register a block: `notifier_chain_register`
```c
static int notifier_chain_register(struct notifier_block **nl,
		                   struct notifier_block *n)
static int notifier_chain_unregister(struct notifier_block **nl,
		                     struct notifier_block *n)
```
However, wrappers exist for this functions, for example:
- Keyboard, on `linux/keyboard.h`
```c
extern int register_keyboard_notifier(struct notifier_block *nb);
extern int unregister_keyboard_notifier(struct notifier_block *nb);
```
- Inet on `linux/inetdevice.h`
```c
int register_inetaddr_notifier(struct notifier_block *nb);
int unregister_inetaddr_notifier(struct notifier_block *nb);
```
- Shutdown on `linux/reboot.h`
```c
extern int register_reboot_notifier(struct notifier_block *);
extern int unregister_reboot_notifier(struct notifier_block *);
```
And much more, all of them of the type **`register_XXX_notifier`**.





