## PrintK
---
### Code
---
```c
#define pr_fmt(fmt)	KBUILD_MODNAME "->%s:%d: " fmt, __func__, __LINE__

#include <linux/module.h>
#include <linux/init.h>

static int __init lkm_init(void)
{
	printk(KERN_INFO "This is KERN_INFO log level\n");
	pr_info("Hello Info\n");

	printk(KERN_ALERT "This is KERN_ALERT log level\n");
	pr_alert("Hello Alert\n");

	printk(KERN_EMERG "This is KERN_EMERG log level\n");
	pr_emerg("Hello Emergency\n");

	return 0;
}

static void __exit lkm_exit(void)
{
	pr_info("Exiting... Bye World\n");
}

module_init(lkm_init);		//<- Tells that Init Kernel Module is the lkm_init function
module_exit(lkm_exit);		//<- Tells that Exit Kernel Function is lkm_exit

MODULE_AUTHOR("Guillem Alminyana");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("LKM Example");
```
From the HelloWorld.c example, `lkm_init` is recoded to show the basic usage of `printk()`. The log level must be specified, and then the text to print. Just below each `printk()` call, the equivalent `pr_*` macro is used. When module is inserted the logs are:
```bash
LKM# dmesg
...[REMOVED]...
[ 2375.134449] This is KERN_INFO log level
[ 2375.134453] helloworld->lkm_init:17: Hello Info
[ 2375.134454] This is KERN_ALERT log level
[ 2375.134460] helloworld->lkm_init:20: Hello Alert
[ 2375.134462] This is KERN_EMERG log level
[ 2375.134465] helloworld->lkm_init:23: Hello Emergency
LKM#
```
### printk.h
---
```c
/*
 * These can be used to print at the various log levels.
 * All of these will print unconditionally, although note that pr_debug()
 * and other debug macros are compiled out unless either DEBUG is defined
 * or CONFIG_DYNAMIC_DEBUG is set.
 */
#define pr_emerg(fmt, ...) \
	printk(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
#define pr_alert(fmt, ...) \
	printk(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
#define pr_crit(fmt, ...) \
	printk(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...) \
	printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warning(fmt, ...) \
	printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn pr_warning
#define pr_notice(fmt, ...) \
	printk(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
#define pr_info(fmt, ...) \
	printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
```

### kern_levels.h
---
```c
#define KERN_SOH	"\001"		/* ASCII Start Of Header */
#define KERN_SOH_ASCII	'\001'

#define KERN_EMERG	KERN_SOH "0"	/* system is unusable */
#define KERN_ALERT	KERN_SOH "1"	/* action must be taken immediately */
#define KERN_CRIT	KERN_SOH "2"	/* critical conditions */
#define KERN_ERR	KERN_SOH "3"	/* error conditions */
#define KERN_WARNING	KERN_SOH "4"	/* warning conditions */
#define KERN_NOTICE	KERN_SOH "5"	/* normal but significant condition */
#define KERN_INFO	KERN_SOH "6"	/* informational */
#define KERN_DEBUG	KERN_SOH "7"	/* debug-level messages */

#define KERN_DEFAULT	KERN_SOH "d"	/* the default kernel loglevel */
```
This log levels must be tagged in the `printk` call.

