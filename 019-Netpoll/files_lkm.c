#include <linux/module.h>
#include <linux/kernel.h>
static struct netpoll np_t;
static struct netpoll * np_p;

static unsigned char buffer[] = "Sending UDP packet\n";

static int __init lkm_init(void)
{
        pr_info("Loading Module.\n");

        np_t.name = "Gui";

        strlcpy(np_t.dev_name, "enp0s3", IFNAMSIZ);

        np_t.local_ip.ip = htonl(0xc0a8013d);  //<- 192.168.1.45
        np_t.local_ip.in.s_addr = htonl(0xc0a8013d);  //<- 192.168.1.45

        np_t.remote_ip.ip = htonl((unsigned long int)0xc0a8012c); //192.168.1.1
        np_t.remote_ip.in.s_addr = htonl((unsigned long int)0xc0a8012c); //192.168.1.1

        np_t.ipv6 = 0;                    //no IPv6

        np_t.local_port = 6666;
        np_t.remote_port = 514;

        memset(np_t.remote_mac, 0xff, ETH_ALEN);

        //netpoll_print_options(&np_t);
        netpoll_setup(&np_t);
        np_p = &np_t;

        netpoll_send_udp(np_p, buffer, strlen(buffer));

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
MODULE_DESCRIPTION("LKM Netpoll Example");
