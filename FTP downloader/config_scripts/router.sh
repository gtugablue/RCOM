#Configuration for router
#Reiniciar o router
copy flash:tux<BANCADA>-clean startup-config
reload
#fazer configurações do guião e guardar com
copy running-config flash:<turna-nome1-nome2-nome3>
#reiniciar usando mesmos passos mas novo ficheiro em vez de tuxy-clean

#Experiment 4
conf t
interface gigabitethernet 0/0
ip address 172.16.<BANCADA>1.254 255.255.255.0
no shutdown
ip nat inside
exit

interface gigabitethernet 0/1
ip address 172.16.1.19 255.255.255.0
no shutdown
ip nat outside
exit

ip nat pool ovrld 172.16.1.19 172.16.1.19 prefix 24
ip nat inside source list 1 pool ovrld overload

access-list 1 permit 172.16.<BANCADA>0.0 0.0.0.7
access-list 1 permit 172.16.<BANCADA>1.0 0.0.0.7

ip route 0.0.0.0 0.0.0.0 172.16.<BANCADA>1.254
ip route 172.16.<BANCADA>0.0 255.255.255.0 172.16.<BANCADA>1.253
end

#		FALTA ADICIONAR ROUTE PARA 172.16.<BANCADA>0.0/24

#		ADICIONAR CAPACIDADE NAT AO ROUTER