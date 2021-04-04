#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static char buffer_to_wr[256] = "Hello World";            //<- buffer to print to file
static char buffer_to_rd[256];                            //<- store what¡s readed from file

static int __init lkm_init(void)
{

        struct file * file;                           //<- file struct pointer
        mm_segment_t fs;                              //<- to save segment
        loff_t pos;

        file = filp_open("/root/test_text.txt",       //<- file creation
                         O_RDWR|O_CREAT, 0700);

        if (IS_ERR(file))                            //<- Error checking
        {
                pr_err("  Error opening file.\n");
                return -1;
        }
        pr_info("   File Opened.\n");

        fs = get_fs();                               //<- Save the current segment: KERNEL_DS or USER_DS
        set_fs(KERNEL_DS);                           //<- Set to use kernel addresses

        pos = 0;                                     //<- Initialize

        vfs_write(file, buffer_to_wr, sizeof(buffer_to_wr), &pos);
        pr_info("   Writed to file.\n");

        pos = 0;                                    //<- Initialize

        vfs_read(file, buffer_to_rd, sizeof(buffer_to_rd), &pos);
        pr_info("   Readed: %s.\n", buffer_to_rd);

        filp_close(file, NULL);                    //<- Close the file
        pr_info("   File Closed.\n");

        return 0;
}

static void __exit lkm_exit(void)
{
        pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Keyb Notification Chains Example");