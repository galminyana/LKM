## Process Info 01
---

LEt's explore Internal Data Structures in the Kernel, where information for Processes is stored.

In user space a Process has a Main thread (`main()` function), and depending on the process it spawns Child Threads. In Kernel Space, Kernel mantains a Task List, and inside, the Main Thread and each Child Thread is treated as a Task.

| User Space ||| Kernel Space |||
|-|-|-|-|-|-|
|    Main Thread ||| Task List |||
|  ChildThread 1 | Child Thread 2 || Main Thread | Child Thread 1| Child Thread X |



### 
---
