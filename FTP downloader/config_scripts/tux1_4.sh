#Configuration for tux1_4

#Experiment 1
#	Delete previous configurations
/etc/init.d/networking restart
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
ifconfig eth0 down
ifconfig eth1 down
ifconfig eth2 down
ifconfig ath0 down

#	Configure eth0
ifconfig eth0 172.16.10.254/24