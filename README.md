
# Remote Emergency System

Primeiramente, vamos falar sobre o que é o Remote Emergency System (RES). Trata-se de um sistema obrigatório para o carro autônomo, que devemos seguir pelo regulamento da [Formula Student Germany](https://www.formulastudent.de/). Mas o que ele realmente faz?

Um carro que anda de forma autônoma deve ter uma grande redundância de equipamentos para segurança. Em um carro feito por estudantes, isso é ainda mais importante. Pensando nisso, foi desenvolvido um sistema que possa ligar e desligar o carro a uma grande distância e que, ao ser desligado, ative os freios de emergência (EBS).

## 🛠️Funcionamento 

Considerando que um sistema de segurança deve funcionar da melhor forma possível, um meio de comunicação muito bom é o **LoRa**. Trata-se de um sistema que opera em baixa frequência, diminuindo assim a interferência e aumentando a distância de funcionamento.

Para o sistema funcionar utilizamos duas placas, sendo uma delas o sender que é o dispostivo que deve enviar os dados, e temos o receiver, o responsavel de receber os dados enviados pelo sender. 

O sender envia tres mensagens sendo elas: 
- Emergency 
- R2D (Ready-to-drive)
- Go (A missão pré-selecionada do carro deve começar)

O receiver ira receber essas mensagens e deve seguir um padrão de atividades, sendo elas:
- Emergency (O Shutdown Circuit deve abrir que esse botão for pressionado e o EBS(Emergency Brake System) deve ser ativado)
- R2D (Esse sinal deve ser enviado quando o carro estiver pronto, após receber esse sinal ele deve esperar 5 segundos para receber o próximo sinal)
- Go (Deve mandar uma mensagem via CAN para que os dispositivos saibam que devem começar a dirigir

Sabendo dos sinais que cada dispositivo deve utilizar, devemos saber como implantar essa funcionalidades é quais são as regras de utilização, para o funcionamento do mesmo, para isso vamos implementar uma Máquina de estado finita (FSM). E como deve funcionar nossos estados:

[[FSM RES.png]]

O reset é um método de teste que não deve ser implementado no sistema final sem antes consultar o regulamento.

## 💀 Implementação STM32

Estou utilizando a núcleo [STM32WL55Cl](https://www.st.com/content/ccc/resource/training/technical/product_training/group1/dd/f1/d0/1a/d0/da/42/24/STM32WL-Ecosystem-STM32WL_Boards_BOARD/files/STM32WL-Ecosystem-STM32WL_Boards_BOARD.pdf/_jcr_content/translations/en.STM32WL-Ecosystem-STM32WL_Boards_BOARD.pdf), antes de começar de fato a programação é necessário algumas configurações, estou usando como referencia o blog do [Matt_Mielke](https://forum.digikey.com/t/using-the-low-level-sub-ghz-radio-driver-for-the-stm32wl-series/18253), vamos separar em passos para facilitar a aplicação.

1. Criar um projeto com a placa de utilização, no meu caso criei com a placa STM32WL55JCI7
2. Quando for fazer o setup da criação deve desmarcar a opção *Enable Multi Cpus Configuration*
3. Ativar os pinos para comunicação UART, vamos colocar em modo de funcionamento *Asynchronous*
4. Ativar o modulo *SUBGHZ* e vamos fazer a seguintes configurações:
    - *Baudrate Prescaler Value*  = 4
    - *SUBGHZ Radio interrupt = Enabled*
5. Devemos ir na aba de *Clock Configuration* e mudar o *MSI RC* para 48000
6. Finalmente vamos na pagina *Project Manager* em *Code Generator* e vamos ativar *Generate peripheral initialization as a pair of ‘.c/.h’ files per peripheral*

Agora temos todas configurações do projeto, mas como a comunicação LoRa se trata de um protocolo devemos seguir um padrão de envio de mensagens, para facilitar temos algumas bibliotecas que fazem as configurações de inicialização do radio, modulação, entre outras configurações do protocolo.

Para instalação da biblioteca que vamos utilizar,  vamos começar  instalando alguns pacotes que o STM nos fornece, chamado [STM32CubeWL](https://www.st.com/en/embedded-software/stm32cubewl.html), no momento que foi escrito esse documento, foi utilizado a versão 1.3.0. Após baixado esse package, vamos utilizar alguns códigos.
1. Vamos começar importando a pasta BSP, ela deve ser importada dentro de Drivers do nosso projeto, o caminho dela é STM32Cube_FW_WL_V1.3.0\Drivers\BSP, dentro da pasta BSP vamos querer apenas a pasta STM32WLxx_Nucleo
2. Devemos mudar o nome do arquivo `stm32wlxx_nucleo_conf_template.h` para `stm32wlxx_nucleo_conf.h`
3. Agora na raiz do nosso projeto devemos criar uma pasta chamada *Utils*, essa pasta deve ser criado como uma *Source folder* 
4. E vamos importar os seguintes arquivos nessa pasta:
	- `Utilities/conf/utilities_conf_template.h`
	- `Utilities/misc/stm32_mem.c`
	- `Utilities/misc/stm32_mem.h`
5. Novamente devemos renomear o arquivo `utilities_conf_template.h` para `utilities_conf.h`
6. Dentro de *Drivers* vamos criar uma pasta chamada Radio e vamos adicionar alguns arquivos:
	- `Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver/radio_driver.c`
	- `Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver/radio_driver.h`
	- `Middlewares/Third_Party/SubGHz_Phy/Conf/radio_conf_template.h`
	- `Projects/NUCLEO-WL55JC/Applications/SubGHz_Phy/SubGHz_Phy_PingPong/SubGHz_Phy/Target/radio_board_if.c`
	- `Projects/NUCLEO-WL55JC/Applications/SubGHz_Phy/SubGHz_Phy_PingPong/SubGHz_Phy/Target/radio_board_if.h`
7. Renomear o arquivo `radio_conf_template.h` para `radio_conf.h`
8. Agora vamos abrir o arquivo `radio_conf.h` e comentar (ou deletar) os *include* `mw_log_conf.h`, `utilities_def.h` e `sys_debug.h`
9. Similarmente para o arquivo `radio_driver.c`, mas devemos retirar o `mw_log_conf.h`
10. Vamos importar dentro de *Core/Inc* o arquivo `Projects/NUCLEO-WL55JC/Applications/SubGHz_Phy/SubGHz_Phy_PingPong/Core/Inc/platform.h`
11. E novamente devemos comentar (ou deletar) o *include* `stm32wlxx_ll_gpio.h`
12. Agora em todas as pastas que criamos devemos conferir se ela esta no caminho de *build* e *debug* , para isso clique com o direito sobre a pasta e vá até *Add/Remove include directory* e veja se ambas opções estão marcadas.
13. Por fim podemos ir na `main.c` do nosso projeto e colocar `#include "radio_driver.h"`

Se você quiser pular todas essas etapas e apenas utilizar o código a partir desse momento, você pode apenas utilizar os seguintes comandos:

```
git clone https://github.com/amperaufsc/remote-emergency-system.git
cd remote-emergency-system
git checkout d86d93bc04566cec1e76f70f084b3db4bfa4a136
```

A partir desse momento você pode começar a criar seu código implementando suas funções de comunicação. O [Matt_Mielke](https://github.com/eewiki/stm32wl_radioDriver_pingPong) deixa um código de PingPong no seu GitHub.

Após toda a configuração, podemos começar a fazer o nosso código. Como sabemos que o código deve implementar a comunicação com duas placas a uma certa distância, devemos criar o código para a comunicação.

Para isso, criei um [diretório](https://github.com/amperaufsc/remote-emergency-system/tree/main/LoRaWizard) para melhor organização. Irei comentar um pouco sobre cada arquivo:

- *LoRaConf.c/h*: Nesse arquivo, faço as configurações iniciais para o projeto e, ao mesmo tempo, crio funções que servem para que outro usuário possa mudar as configurações seguindo as regras pré-estabelecidas pelo dispositivo. Há também uma função chamada setStateMode que irá definir se o dispositivo é o Master (sender) ou o Slave (receiver).
- *LoRaEvents.c/h*: Neste arquivo, setamos todos os eventos que podem ocorrer na comunicação, como falha, timeout, sucesso (OK) e padrão (default).
- *LoRaMessageHandler.c/h*: Um dos arquivos mais importantes. Aqui, implementamos a comunicação do dispositivo, tanto o envio de mensagens pela função SendMessage quanto o recebimento pela receiveMessage.
- *mainLoRa.c/h*: Por fim, deixamos todo o código aplicado na mainLoRa, pois isso facilita a escrita e nos permite separar a funcionalidade.

Agora, especificando o que fazemos na mainLoRa:

Primeiramente, desabilito todas as interrupções para que não ocorra nenhuma falha nas configurações. Após isso, faço tudo o que é necessário para deixar a comunicação funcional, como configurar o master ou o slave, definir os padrões do protocolo usado, etc. Depois disso, habilito novamente as interrupções e cada código individual começa a ser executado.

#### Master

Ele sempre envia a mensagem para se conectar e, ao receber uma alteração no botão, muda de estado, seguindo a ordem pré-estabelecida, e envia a nova condição para o receiver.

#### Slave

Neste ponto, mais ações são realizadas. Primeiramente, o sistema sempre aguarda o recebimento de uma mensagem. Após recebê-la, ele faz uma verificação para ver se ela corresponde a um dos comandos esperados. Em caso afirmativo, o estado do carro é alterado e a ação destinada àquela mensagem é executada. 

## 💊 Melhorias 

Como podemos notar, o projeto ainda não está finalizado. Acredito que a comunicação pode ser mais otimizada e segura, além de que o RES ainda não é capaz de enviar a mensagem via CAN para que o carro a receba.

Para facilitar os próximos passos, criarei uma checklist com as melhorias a serem feitas:

- [ ] Garantir a comunicação LoRa. No código, podemos ver a utilização de um delay para o recebimento da mensagem, o que não é ideal e deve ser removido.
- [ ] Implementar duas máquinas de estados finitos para garantir que cada dispositivo saiba o momento certo de enviar e receber mensagens. Isso irá melhorar a comunicação, torná-la mais assertiva, eliminar o uso de delays e aprimorar a implementação da máquina de estados do carro.
- [ ] Segurança no envio de mensagens: utilizar uma criptografia simples no envio e recebimento para garantir que terceiros não consigam enviar comandos ao carro, mesmo com a presença de outras pessoas nas proximidades.
- [ ] Implementação do CAN de forma assertiva e seguindo o regulamento.
- [ ] Testar as funções criadas.
- [ ] Utilizar os códigos de eventos.

## 📁 Acesso ao projeto
Você pode acessar os arquivos do projeto clicando [aqui](https://github.com/amperaufsc/remote-emergency-system).

## Desenvolvedores
| [<img src="https://avatars.githubusercontent.com/u/109047608?s=96&v=4" width=115><br><sub>Kakasten</sub>](https://github.com/kakasten) |
| :---: |
