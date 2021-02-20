## Process Info 01
---

LEt's explore Internal Data Structures in the Kernel, where information for Processes is stored.

In user space a Process has a Main thread (`main()` function), and depending on the process it spawns Child Threads. In Kernel Space, Kernel mantains a Task List, and inside, the Main Thread and each Child Thread is treated as a Task.

| User Space ||| Kernel Space |||
|-|-|-|-|-|-|
|    Main Thread ||| Task List |||
|  ChildThread 1 | Child Thread 2 || Main Thread | Child Thread 1| Child Thread X |



### User vs Kernel Space
---

| User Space Process | Kernel Space Tasks |
|-|-|
| Only has access to own Virtual Memory | Has access to any memory in Kernel Space |
| Can not access other processes space | Access to everything |
| PID is the Process ID | TGID is the Thread Group ID that identifies Main Thead |
| Thread ID identifies threads | PID identifies Task ID. Main Thread TGID == PID and for Other Threads TGID != PID |
| Easy to dump mempry | Need to lock memory before access to it |
