#Configuration for switch

#Experiment 2
#	Delete vlans 10, 11
configure terminal
no vlan 10
end
configure terminal
no vlan 11
end
#	Configure vlans 10, 11
configure terminal
vlan 10
end
configure terminal
vlan 11
end
#	Add ports to vlans
#		Add port 1 to vlan 10
configure terminal
interface fastethernet 0/1
switchport mode access
switchport access vlan 10
end
#		Add port 4 to vlan 10
configure terminal
interface fastethernet 0/4
switchport mode access
switchport access vlan 10
end
#		Add port 2 to vlan 11
configure terminal
interface fastethernet 0/2
switchport mode access
switchport access vlan 11
end