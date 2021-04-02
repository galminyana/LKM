#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/keyboard.h>


static int my_keys (struct notifier_block *nb, unsigned long action, void * data)
{
        struct keyboard_notifier_param * knd = data;     //<- Get the data for the callback into "knd"

        if ((action == KBD_KEYSYM) && (knd->down))       //<- Check if the callback happened because a ascii key pressed
        {
                pr_info("   Pressed key '%c' with value '%d'.\n", knd->value, knd->value);
        }

        return NOTIFY_OK;
}

/*
 Declare and Initialize the notifier_block with th Callback function
*/
static struct notifier_block nb = {
        .notifier_call = my_keys                      //<- Assign the callback function to this notification chain
};

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        register_keyboard_notifier(&nb);             //<- register the keyboard notification chain
                                                     //   for the block that already been created

        pr_info("   Registered Keyboard Notifier.\n");

        pr_info("Module Loaded.\n");
        return 0;
}

static void __exit lkm_exit(void)
{

        unregister_keyboard_notifier(&nb);            //<- Unregister the keyboard notifier

        pr_info("   Unregistered Keyboard Notifier.\n");

        pr_info("Exiting... Bye.\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Keyb Notification Chains Example");
