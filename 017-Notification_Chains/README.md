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
		                   struct notifier_block *n);
static int notifier_chain_unregister(struct notifier_block **nl,
		                     struct notifier_block *n);
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

### Keyboard Notification Chains: Example
---
Have to:
- Create a `notifier_block` struct
- Create Callback Function: 
 - Keyboard data for the callback function needs a `keyboard_notifier_param`
 - Return right value, described before possible values
- Register the Keyboard Notifier using `register_keyboard_notifier`

### Callback Function
```c
typedef int (*notifier_fn_t)(struct notifier_block *nb, 
                             unsigned long action, 
                             void *data);
```
### Data Parameter: `keyboard_notifier_param`
Used to pass data related to key pressing. It's defined on `linux/keyboard.h`
```c
struct keyboard_notifier_param {
	struct vc_data *vc;	/* VC on which the keyboard press was done */
	int down;		/* Pressure of the key? */
	int shift;		/* Current shift mask */
	int ledstate;		/* Current led state */
	unsigned int value;	/* keycode, unicode value or keysym */
};
```
Interesting values from the struct:
- `value`: Value for the key pressed
- `down`: Status of the key, 1 key pressed, 0 key released

### Action Parameter
Describes the type of the event. For Keyboard Notification Chains can be any of thoses (defined in `linux/notifier.h`:
```c
/* Console keyboard events.
 * Note: KBD_KEYCODE is always sent before KBD_UNBOUND_KEYCODE, KBD_UNICODE and
 * KBD_KEYSYM. */
#define KBD_KEYCODE		0x0001 /* Keyboard keycode, called before any other */
#define KBD_UNBOUND_KEYCODE	0x0002 /* Keyboard keycode which is not bound to any other */
#define KBD_UNICODE		0x0003 /* Keyboard unicode */
#define KBD_KEYSYM		0x0004 /* Keyboard keysym */
#define KBD_POST_KEYSYM		0x0005 /* Called after keyboard keysym interpretation */
```
Where the event that returns printable ASCII characters is `KBD_KEYSYM`.

### References
---

- [Post by **hasp0t** at **0x00sec.org**](https://0x00sec.org/t/linux-keylogger-and-notification-chains/4566)
- [Linux Inside, Notification Chains Chapter](https://0xax.gitbooks.io/linux-insides/content/Concepts/linux-cpu-4.html)



