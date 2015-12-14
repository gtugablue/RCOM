#Configuration for switch
#Reiniciar o switch
del flash:vlan.dat
copy flash:tux<BANCADA>-clean startup-config
reload
#fazer configurações do guião e guardar com
copy running-config flash:<turna-nome1-nome2-nome3>
#reiniciar usando mesmos passos mas novo ficheiro em vez de tuxy-clean

#Experiment 2
#	Delete vlans <BANCADA>0, <BANCADA>1
configure terminal
no vlan <BANCADA>0
end
configure terminal
no vlan <BANCADA>1
end
#	Configure vlans <BANCADA>0, <BANCADA>1
configure terminal
vlan <BANCADA>0
end
configure terminal
vlan <BANCADA>1
end
#	Add ports to vlans
#		Add port 1 to vlan <BANCADA>0
configure terminal
interface fastethernet 0/1
switchport mode access
switchport access vlan <BANCADA>0
end
#		Add port 4 to vlan <BANCADA>0
configure terminal
interface fastethernet 0/4
switchport mode access
switchport access vlan <BANCADA>0
end
#		Add port 2 to vlan <BANCADA>1
configure terminal
interface fastethernet 0/2
switchport mode access
switchport access vlan <BANCADA>1
end

#Experiment 3
#	Add secondary tux4 port to vlan <BANCADA>1
configure terminal
interface fastethernet 0/8
switchport mode access
switchport access vlan <BANCADA>1
end

#Experiment 4
#	Add router port to vlan <BANCADA>1
configure terminal
interface fastethernet 0/9
switchport mode access
switchport access vlan <BANCADA>1
end
