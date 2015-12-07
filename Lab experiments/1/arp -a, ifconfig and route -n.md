## tux14

# arp -a
 (172.16.10.1) at 00:0f:fe:8b:e4:ef [ether] on eth0

# ifconfig
eth0      Link encap:Ethernet  HWaddr 00:22:64:a6:a4:f8  
          inet addr:172.16.10.254  Bcast:172.16.10.255  Mask:255.255.255.0
          inet6 addr: fe80::222:64ff:fea6:a4f8/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:51 errors:0 dropped:1056 overruns:0 frame:0
          TX packets:69 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:4214 (4.1 KiB)  TX bytes:12110 (11.8 KiB)
          Interrupt:17 

#route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
172.16.10.0     0.0.0.0         255.255.255.0   U     0      0        0 eth0