# Trabalho Final - FSE - Sistema Sistribuído de Automação Residencial 
aluno: Lucas Penido Antunes  
matrícula: 16/0013143

Repositório com a solução do Servidor do Trabalho Final.

## Projeto

O objetivo deste trabalho é criar um sistema distribuído de automação residencial utilizando como sistema computacional central uma placa Raspberry Pi 4 e como controladores distribuídos placas ESP32, interconectados via Wifi através do protocolo MQTT.

Para uma maior descrição do projeto visitar este [GitLab](https://gitlab.com/fse_fga/projetos/trabalho-final).

### Executando o programa

Execute:  

`make` 

Após a criação do executável:

`make run`

Para limpar o projeto:

`make clean`

Obs: Os dados serão armazenados no arquivo "dados_acionamento.csv" na mesma pasta.

### Utilização

Após a execução do servidor deve-se ligar os clientes, aparecerá uma tela onde será possível realizar o controle dos dispositívos, monitorar sensores, ativar o alarme, além de obter informações sobre a temperatura e umidade.  
É possível também monitorar e ativar os clientes cadastrados

Para acionar qualquer dispositive do servidor digite o número 1 ou 2.  
Para acionar o dispositivos de saída dos clientes aperte a letra designada para ele (O primeiro cliente cadastrado terá a letra "d").
Para ligar/desligar o alarme digite o número 7.  
Para sair do programa digite o número 0.  

### Tela de Controle
![tela](https://i.imgur.com/1XPSu4O.png)

### Cadastro de Cliente
Quando um cliente estiver pedindo para se cadastrar, aparecerá uma notificação no menu de notificações. Para cadastrar o cliente aperte "c".  
![notificações](https://i.imgur.com/KvrzoqU.png)

Assim, aparecerá um menu para que você possa cadastrá-lo  
![menu cadastro](https://i.imgur.com/thtAqIw.png)
