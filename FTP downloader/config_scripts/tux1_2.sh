#!/bin/bash
#Configuration for tux1_2

#Experiment 2
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
ifconfig eth0 172.16.<BANCADA>1.0/24

#Experiment 3
route add default gw 172.16.<BANCADA>1.253
arp -d 172.16.<BANCADA>1.1
arp -d 172.16.<BANCADA>1.253

#Experiment 4
#	Make default route to rc
route add default gw 172.16.<BANCADA>1.254
route add -net 172.16.<BANCADA>0.0/24 gw 172.16.61.253

echo 0 > /proc/sys/net/ipv4/conf/eth0/accept_redirects
echo 0 > /proc/sys/net/ipv4/conf/all/accept_redirects

# FALTA REMOVER ROUTE PARA 172.16.<BANCADA>0.0/24 via tux4
# READICIONAR ROUTE ^