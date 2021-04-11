#include <linux/module.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

static int times_task_executed = 0;                         //<- To hold hoy many times task been executed

static void scheduled_task (struct work_struct * work);     //<- Function prototipe definition

static int stop = 0;                                        //<- When "1", stop running the task

static struct workqueue_struct * my_workqueue;

static DECLARE_DELAYED_WORK(my_task, scheduled_task);       //<- Declare and assign the function to the task
                                                            //   Defines a work_struct called my_task
static void scheduled_task (struct work_struct * work)
{
        pr_info("   Executed task %d times.\n", times_task_executed);

        times_task_executed++;

        if (stop == 0)                                      //<- Still don't stop
        {
                queue_delayed_work(my_workqueue, &my_task, 100);
        }
}

static int __init lkm_init (void)
{
        pr_info("Loading Module.\n");
        
        my_workqueue = create_workqueue("my_Queue_name");
        queue_delayed_work(my_workqueue, &my_task, 100);

        return 0;
}

static void __exit lkm_exit (void)
{
        stop = 1;                        //<- Tell the qork queue to stop execution

        cancel_delayed_work(&my_task);
        flush_workqueue(my_workqueue);
        destroy_workqueue(my_workqueue);

        pr_info("Exiting... \n");
}

module_init(lkm_init);          //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);          //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Task Scheduler Example");
