#include <linux/module.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

static int times_task_executed = 0;                          //<- To hold hoy many times task been executed

static void scheduled_task (struct work_struct * work);     //<- Function prototipe definition

static struct workqueue_struct * my_workqueue;
static struct work_struct my_task;

static DECLARE_WORK(my_task, scheduled_task);               //<- Declare and assign the function to the task

static void scheduled_task (struct work_struct * work)
{
        pr_info("   Executed task %d times.\n", times_task_executed);

        times_task_executed++;

}

static int __init lkm_init (void)
{


        return 0;
}

static void __exit lkm_exit (void)
{


        pr_info("Exiting... \n");
}

module_init(lkm_init);          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Task Scheduler Example");
