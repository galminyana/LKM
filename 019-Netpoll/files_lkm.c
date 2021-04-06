#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netpoll.h>

static struct netpoll np_t;
static struct netpoll * np_p;

static unsigned char buffer[] = "Sending UDP packet.\n";

static int __init lkm_init(void)
{
        pr_info("Loading Module.\n");

        np_t.name = "MWM";

        pr_info("   Set the netpoll name to the interface.\n");
        strlcpy(np_t.dev_name, "enp0s3", IFNAMSIZ);

        pr_info("   Set the Local IP Address.\n");
        np_t.local_ip.ip = htonl(0xc0a8013d);                            //<- For 192.168.1.45
        np_t.local_ip.in.s_addr = htonl(0xc0a8013d);                     //<- 192.168.1.45

        pr_info("   Set the Remote IP Address.\n");        
        np_t.remote_ip.ip = htonl((unsigned long int)0xc0a8012c);        //<- For 192.168.1.44
        np_t.remote_ip.in.s_addr = htonl((unsigned long int)0xc0a8012c); //<- For 192.168.1.44

        pr_info("   Disable IPv6.\n");        
        np_t.ipv6 = 0;             

        pr_info("   Set the Local and Remote UDP Ports.\n");        
        np_t.local_port = 6666;
        np_t.remote_port = 514;

        pr_info("   Set the MAC Address to broadcast.\n");        
        memset(np_t.remote_mac, 0xff, ETH_ALEN);                       //<- 0xff bytes set ETH_ALEN times, 
                                                                       //   means 0xffffffffffff

        //netpoll_print_options(&np_t);                                //<- Prints bunch of debugging and info data 
        
        pr_info("   Set Up the netpoll struct.\n");
        netpoll_setup(&np_t);                                          
        
        np_p = &np_t;                                                  //<- Make a pointer to the netpoll struct

        pr_info("   Sending the UDP packer.\n");
        netpoll_send_udp(np_p, buffer, strlen(buffer));                //<- Send the buffer over the netpoll struct

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
