#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#define DEVICE_CLASS "my_char_device"
#define DEVICE_NAME  "my_char"

int number_major = 0;                                                  //<- Major number for the Device
static int times_opened = 0;                                           //<- Times the device been opened
static unsigned char message[20] = "Reading from Device";              //<- Buffer to store text comming from user space
static int message_size = 20;                                          //<- Size of the buffer
static struct device* device_struct = NULL;                            //<- Device Driver struct pointer. To create the /dev/FILE
static struct class*  device_class =  NULL;                            //<- Device Driver class struct pointer

static int dev_open(struct inode *pinode, struct file *pfile)
{
        times_opened++;
        pr_info("Device /dev/%s opened %d times.\n", DEVICE_NAME, times_opened);
        return 0;
}

static int dev_close( struct inode *pinode, struct file *pfile)
{
        pr_info("Device /dev/%s closed.\n", DEVICE_NAME);
        return 0;
}

static ssize_t dev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
        pr_info("User Reading from char device\n");

        return 0;
}

static ssize_t dev_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
        pr_info("User Writing to device\n");
        return 0;
}

struct file_operations dev_file_operations = {
        .owner = THIS_MODULE,
        .open = dev_open,
        .read = dev_read,
        .write = dev_write,
        .release = dev_close,
};

static int __init lkm_init(void)
{
        pr_info("Loading Module...\n");

        number_major = register_chrdev(0, DEVICE_NAME, &dev_file_operations);  //<- Register Device. Returns the major number
                                                                               //   Allocates it dinamically as param#1 is "0"
        if (number_major < 0) {                                                //<- Error checking
                pr_err("   Error registering device\n");
                return number_major;
        } else {
                pr_info("   Registered device with major number %d.\n", number_major);
        }

        device_class = class_create(THIS_MODULE, DEVICE_CLASS);                //<- Registers the device class.
 
        if (IS_ERR(device_class)){                                             //<- Error checking
                unregister_chrdev(number_major, DEVICE_NAME);                  //<-- Cleanup
                pr_err("   Error registering device class. Cleaning...\n");
        } else {
                pr_info("   Registered device class\n");
        }

        device_struct = device_create(device_class,NULL,                       //<- Register Device Driver
                                      MKDEV(number_major, 0),
                                      NULL,"mychar");  
        if (IS_ERR(device_struct)){                                            //<- Error checking for cleanup
                class_destroy(device_class);                                   //<-- Cleanup device class
                unregister_chrdev(number_major, DEVICE_NAME);                  //<-- Cleanup registered device
                pr_err("   Error creating Device driver. Cleaning...\n");
        } else {
                pr_info("   Created Device Driver\n");
        }
 
        pr_info("Done initializing module.\n");
        return 0;
}

static void __exit lkm_exit(void)
{
        device_destroy(device_class, MKDEV(number_major, 0));                   //<- Remove the device
        class_unregister(device_class);                                         //<- Unregister the device class
        class_destroy(device_class);                                            //<- Remove the device class
        unregister_chrdev(number_major, DEVICE_NAME);                           //<- Unregister the major number 
        pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);                                                                                  //<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);                                                                                  //<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("LKM Char Device Example");


