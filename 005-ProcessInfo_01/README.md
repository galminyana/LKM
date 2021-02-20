## Process Info 01
---

Exploring internal data structures in the Kernel. In this case the structure where process information is stored.

It's important to understand how the processes are internally represented in the Kernel, and modify and access to that information.

In user space we have a Process, that has a Main thread (main function), and depending on the process it spawns Child threads. 

In Kernel Space is different. Kernel mantains a Task List, and inside, the Main Thread and each Child Thread is a Task.

| User Space ||| Kernel Space |
|-|-|-|-|
|    Main Thread ||| Task List |||
|  ChildThread 1 | Child Thread 2 || Main Thread | Child Thread 1| Child Thread X |



### 
---
