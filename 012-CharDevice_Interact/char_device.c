#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>


#define DEVICE_CLASS "my_char_device"
#define DEVICE_NAME  "my_char"

int number_major = 0;                                                  //<- Major number for the Device
static struct device* device_struct = NULL;                            //<- Device Driver struct pointer. To create the /dev/FILE
static struct class*  device_class =  NULL;                            //<- Device Driver class struct pointer

static char message[256] = "Hello World";                               //<- Message to print
static int message_length;

static DEFINE_MUTEX(my_char_mutex);                                     //<- Initializes a mutex

static int dev_open (struct inode *pinode, struct file *pfile)
{
        if (!mutex_trylock(&my_char_mutex))                             //<- Locking access to device. If busy, error
        {
                pr_err("Device Busy. Can't lock it.\n");
                return -EBUSY;                                          //<- Resource busy error
        }
  
        pr_info("Device /dev/%s opened %d times.\n", DEVICE_NAME, times_opened);
  
        return 0;
}

static int dev_close (struct inode *pinode, struct file *pfile)
{
        mutex_unlock(&my_char_mutex);                                   //<- Unlock the mutex

        pr_info("Device /dev/%s closed.\n", DEVICE_NAME);
        return 0;
}

static ssize_t dev_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{

        if (*offset > 256)
        {
                return -EFAULT;
        }
        if (*offset + length > 256)
        {
                length = 256 - *offset;
        }

        message_length = copy_to_user(buffer, message + *offset, length);       //<- Copy from Kernel Space to User Space
        if (message_length < 0) {
                pr_err("   Error sending message to user.\n");
                return -EFAULT;
        }

        *offset += length;

        pr_info("   Sent message to user.\n");

        return 0;
}

static ssize_t dev_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
        pr_info("User Writing to device\n");

        message_length = copy_from_user(message, buffer, length);       //<- Move buffer from UserSpace to Kernel Space
        if (message_length < 0) {
                pr_err("   Error reading message from user.\n");
                return -EFAULT;
        }
  
        message_length = length;                                        //<- Not requred as message_legth already has value

        pr_info("   Readed %d characters\n", message_length);

        return 0;
}

struct file_operations dev_file_operations = {                          //<- Initialize the file operations struct
        .owner = THIS_MODULE,                                           //   with our defined functions 
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
                                      NULL, DEVICE_NAME);
        if (IS_ERR(device_struct)){                                            //<- Error checking for cleanup
                class_destroy(device_class);                                   //<-- Cleanup device class
                unregister_chrdev(number_major, DEVICE_NAME);                  //<-- Cleanup registered device
                pr_err("   Error creating Device driver. Cleaning...\n");
        } else {
                pr_info("   Created Device Driver\n");
        }

        mutex_init(&my_char_mutex);                                             //<- Initialize the mutex in runtime

        pr_info("Done initializing module. Created device on /dev/%s.\n", DEVICE_NAME);
        return 0;
}

static void __exit lkm_exit(void)
{
        device_destroy(device_class, MKDEV(number_major, 0));                   //<- Remove the device
        pr_info("   Removed the device.\n");
        class_unregister(device_class);                                         //<- Unregister the device class
        pr_info("   Device Class unregistered.\n");
        class_destroy(device_class);                                            //<- Remove the device class
        pr_info("   Device Class destroyed.\n");
        unregister_chrdev(number_major, DEVICE_NAME);                           //<- Unregister the major number
        pr_info("   Major Number released.\n");
        mutex_destroy(&my_char_mutex);                                          //<- Mutex released
        pr_info("Exiting... Bye.\n");
}

module_init(lkm_init);                                                                                  
module_exit(lkm_exit);                                                                               

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Char Device Example");


