#!/bin/bash
#Configuration for tux1_4

#Experiment 1
#	Delete previous configurations
/etc/init.d/networking restart
chmod +x /proc/sys/net/ipv4/ip_forward
chmod +x /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
ifconfig eth0 down
ifconfig eth1 down
ifconfig eth2 down
ifconfig ath0 down

#	Configure eth0
ifconfig eth0 172.16.10.254/24

#Experiment 3
ifconfig eth1 172.16.11.253
arp -d 172.16.10.1
arp -d 172.16.10.254
arp -d 172.16.11.1
arp -d 172.16.11.253

#Experiment 4
#	Make default route to rc
route add default gw 172.16.11.254