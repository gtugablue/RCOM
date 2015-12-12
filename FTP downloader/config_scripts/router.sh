#Configuration for router
#Reiniciar o router
copy flash:tuxy-clean startup-config
reload
#fazer configurações do guião e guardar com
copy running-config flash:<turna-nome1-nome2-nome3>
#reiniciar usando mesmos passos mas novo ficheiro em vez de tuxy-clean
