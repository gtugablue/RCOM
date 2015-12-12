#!/bin/bash
#Configuration for tux1_1

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

#	Configure eth0 and delete arp entries
ifconfig eth0 172.16.10.1/24
arp -d 172.16.10.1
arp -d 172.16.10.254

#Experiment 3
route add default gw 172.16.10.254
arp -d 172.16.10.1
arp -d 172.16.10.254

#Experiment 4
#	Make default route to tux4
route add default gw 172.16.10.254