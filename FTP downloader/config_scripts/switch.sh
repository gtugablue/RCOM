#Configuration for switch
#Reiniciar o switch
del flash:vlan.dat
copy flash:tux<BANCADA>-clean startup-config
reload
#fazer configurações do guião e guardar com
copy running-config flash:<turna-nome1-nome2-nome3>
#reiniciar usando mesmos passos mas novo ficheiro em vez de tuxy-clean

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

#Experiment 3
#	Add secondary tux4 port to vlan 11
configure terminal
interface fastethernet 0/8
switchport mode access
switchport access vlan 11
end

#Experiment 4
#	Add router port to vlan 11
configure terminal
interface fastethernet 0/9
switchport mode access
switchport access vlan 11
end
