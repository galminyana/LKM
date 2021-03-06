## NetPoll
---
For Kernel > 4.17.0

### `uapi/linux/in.h` -> `in_addr` struct
---
Represents a IP address, formated by `htons`.

```c 
struct in_addr {
	__be32	s_addr;              //<- IP Address. 
};
```
_Note_: For `htons` the input has to be in the format 0xXXYYZZMM to represent the IP Address where XX is the hex value of 1st octet, YY for the second, and so on, example, 192.168.1.1 is 0xc0a80101.

### `linux/netpoll.h` -> `netpoll` struct
---
The required struct, is `netpoll`:
```c
struct netpoll {
	struct net_device *dev;
	char dev_name[IFNAMSIZ];
	const char *name;
	union inet_addr local_ip, remote_ip;
	bool ipv6;
	u16 local_port, remote_port;
	u8 remote_mac[ETH_ALEN];
};
```
Where:
- `local_ip, remote_ip`: To define IP addresses
- `local_port, remote_port`: Define local and remote ports
- `remote_mac`: MAC address to send. Usually `0xFFFFFFFFFFFF` for any

To notice, that `local_ip` and `remote_ip` have to be filled for the IP addresses. Defined as:
```c
union inet_addr {
	__u32		all[4];
	__be32		ip;              //<- IP Address. 
	__be32		ip6[4];
	struct in_addr	in;
	struct in6_addr	in6;
};
```
Where:
- `ip`: IP address, formated by `htons` as explained in a _Note_ before.
- `in`: A `in_addr` struct to represent a IP address

### Set Up Netpoll
---
Define and initialize a netpoll struct
```c
static struct netpoll* np = NULL;
```
Then call the required function to set the netpoll socket using
```c
int netpoll_setup(struct netpoll *np);
```
### Send the Packet
---
```c
void netpoll_send_udp(struct netpoll *np, const char *msg, int len);
```
Where `msg` is the pointer to the message to send, and `len` the length of it.
