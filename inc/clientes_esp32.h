#ifndef CLIENTES_ESP32_H
#define CLIENTES_ESP32_H

#include <stdlib.h>

#define DISPOSITIVO_ENTRADA 0
#define DISPOSITIVO_SAIDA 1

typedef struct cadastro_esp {
    char mac[20];
    char topico[60];
    struct cadastro_esp *prox;
} ListaCadastro;

typedef struct esp_cadastradas {
    char mac[20];
    char topico[60];
    char nomeComodo[50];
    float temperatura;
    float umidade;
    int estado_saida;
    int estado_entrada;
    char teclaRelacionada;
    struct esp_cadastradas *prox;
} ListaEspCadastradas;

extern ListaCadastro *headListaCadastro;
extern ListaEspCadastradas *headListaEspCadastradas;
extern int numCadastro;
extern int numCadastradas;

void adicionaEstadosCliente(int estadoEntrada, int estadoSaida, char *mac);
void adicionaUmidadeCliente(float umidade, char *mac);
void adicionaTemperaturaCliente(float temperatura, char *mac);
void removeEspListaCadastro();
void adicionaListaCadastro(char *macEsp, char *topico);
int contaListaCadastro();
void cadastraEsp(char *nomeComodo);
void acionaSaidaCliente(char teclaAcionamento);

#endif /* MQTT_H */