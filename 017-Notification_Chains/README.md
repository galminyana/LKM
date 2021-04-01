## Notification Chains
---
For Kernel > 4.17.0

Mechanism to allow communication between different Kernel subsystems.

### Related Structures and Functions (`linux/notifier.h`)
---

#### `notifier_block`

```c
struct notifier_block {
	notifier_fn_t notifier_call;           //<- Pointer to Callback function
	struct notifier_block __rcu *next;     //<- Next notifier block of the chain
	int priority;                          //<- Priority of this block in the chain
};
```
Each Kernel subsystem that wants to be notified of an event, must provide it's own `notifier_fn_t` callback function in the `notifier_call` entry of the struct

#### `notifier_call` function declaration

```c
typedef int (*notifier_fn_t)(struct notifier_block *nb, 
                             unsigned long action, 
							 void *data);
```
- `nb`: pointer to the first notification block of the chain 
- `action´: