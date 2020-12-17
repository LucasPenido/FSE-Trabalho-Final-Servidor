#ifndef MQTT_H_
#define MQTT_H_

void enviaMensagemAcionamentoSaida(char* topico, char* mac);
void confirmaCadastramento(char* topico, char* nomeComodo, char* mac);
void* mqtt_conecta();
void mqtt_disconecta();

#endif /* MQTT_H_ */