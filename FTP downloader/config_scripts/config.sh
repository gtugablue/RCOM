#!/bin/bash
#Tux 1_1 configuration
if [ -z $1 ] || [ -z $2 ]; then
	echo "Usage: sh $0 <BANCADA> <TUX>"
else
	# $0 is BANCADA
	# $1 is TUX

	if [ $2 -eq "1" ]; then
		echo "\n\tStarting configuration for tux$1_$2\n"
		echo "\n\tCleaning previous configurations\n"
		/etc/init.d/networking restart
		echo 1 > /proc/sys/net/ipv4/ip_forward
		echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
		ifconfig ath0 down 2> /dev/null	# ignore error messages when interface does not exist
		ifconfig eth0 down 2> /dev/null
		ifconfig eth1 down 2> /dev/null
		ifconfig eth2 down 2> /dev/null

		echo "\n\tExperiment 1\n"
		echo "Configure eth0"
		echo "=> ifconfig eth0 172.16."$1"0.1/24"
		ifconfig eth0 172.16."$1"0.1/24

		echo "\n\tExperiment 3\n"
		echo "Make tux4 (on vlan0) default route"
		echo "=> route add default gw 172.16."$1"0.254"
		route add default gw 172.16."$1"0.254
		arp -d 172.16."$1"0.1 > /dev/null
		arp -d 172.16."$1"0.254 > /dev/null
	fi

	if [ $2 -eq "2" ]; then
		echo "\n\tStarting configuration for tux$1_$2\n"
		echo "\n\tCleaning previous configurations\n"
		/etc/init.d/networking restart
		echo 1 > /proc/sys/net/ipv4/ip_forward
		echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
		ifconfig ath0 down 2> /dev/null	# ignore error messages when interface does not exist
		ifconfig eth0 down 2> /dev/null
		ifconfig eth1 down 2> /dev/null
		ifconfig eth2 down 2> /dev/null

		echo "\n\tExperiment 1\n"
		echo "Configure eth0"
		echo "=> ifconfig eth0 172.16."$1"1.1/24"
		ifconfig eth0 172.16."$1"1.1/24

		echo "\n\tExperiment 4\n"
		echo "Make router default route and add route to vlan0 via tux4"
		echo "=> route add default gw 172.16."$1"1.254"
		route add default gw 172.16."$1"1.254
		echo "=> route add -net 172.16."$1"0.0/24 gw 172.16."$1"1.253"
		route add -net 172.16."$1"0.0/24 gw 172.16."$1"1.253
		#echo 0 > /proc/sys/net/ipv4/conf/eth0/accept_redirects
		#echo 0 > /proc/sys/net/ipv4/conf/all/accept_redirects
	fi

	if [ $2 -eq "4" ]; then
		echo "\n\tStarting configuration for tux$1_$2\n"
		echo "\n\tCleaning previous configurations\n"
		/etc/init.d/networking restart
		echo 1 > /proc/sys/net/ipv4/ip_forward
		echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
		ifconfig ath0 down 2> /dev/null	# ignore error messages when interface does not exist
		ifconfig eth0 down 2> /dev/null
		ifconfig eth1 down 2> /dev/null
		ifconfig eth2 down 2> /dev/null

		echo "\n\tExperiment 1\n"
		echo "Configure eth0 (for vlan0)"
		echo "=> ifconfig eth0 172.16."$1"0.254/24"
		ifconfig eth0 172.16."$1"0.254/24		

		echo "\n\tExperiment 3\n"
		echo "Configure eth1 (for vlan1)"
		echo "=> ifconfig eht1 172.16."$1"1.253/24"		
		ifconfig eth1 172.16."$1"1.253/24

		echo "\n\tExperiment 4\n"
		echo "Make router as default route"
		echo "=> route add default gw 172.16."$1"1.254"
		route add default gw 172.16."$1"1.254
	fi
fi
