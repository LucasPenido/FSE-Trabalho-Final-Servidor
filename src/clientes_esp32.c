#include "clientes_esp32.h"

#include <stdio.h>
#include <string.h>

#include "arquivo.h"
#include "mqtt.h"

ListaCadastro *headListaCadastro;
ListaEspCadastradas *headListaEspCadastradas;
int numCadastro;
int numCadastradas;

void adicionaEstadosCliente(int estadoEntrada, int estadoSaida, char *mac) {
    if (headListaEspCadastradas != NULL) {
        ListaEspCadastradas *temp;
        temp = headListaEspCadastradas;
        while (temp != NULL) {
            if (strcmp(temp->mac, mac) == 0) {
                temp->estado_entrada = estadoEntrada;
                temp->estado_saida = estadoSaida;
                break;
            }
            temp = temp->prox;
        }
    }
}

void adicionaTemperaturaCliente(float temperatura, char *mac) {
    if (headListaEspCadastradas != NULL) {
        ListaEspCadastradas *temp;
        temp = headListaEspCadastradas;
        while (temp != NULL) {
            if (strcmp(temp->mac, mac) == 0) {
                temp->temperatura = temperatura;
                break;
            }
            temp = temp->prox;
        }
    }
}

void adicionaUmidadeCliente(float umidade, char *mac) {
    if (headListaEspCadastradas != NULL) {
        ListaEspCadastradas *temp;
        temp = headListaEspCadastradas;
        while (temp != NULL) {
            if (strcmp(temp->mac, mac) == 0) {
                temp->umidade = umidade;
                break;
            }
            temp = temp->prox;
        }
    }
}

void removeEspListaCadastro() {
    ListaCadastro *temp;
    temp = headListaCadastro;

    headListaCadastro = temp->prox;
    free(temp);
    numCadastro--;
}

/*
*   Remove a primeira esp da lista de cadastro e transfere o mac
*   para a lista de esp's cadastradas.
*/
void cadastraEsp(char *nomeComodo) {
    if (headListaCadastro != NULL) {
        ListaEspCadastradas *espCadastrada = (ListaEspCadastradas *)malloc(sizeof(ListaEspCadastradas));
        ListaCadastro *espListaCadastro = headListaCadastro;

        strcpy(espCadastrada->nomeComodo, nomeComodo);
        strcpy(espCadastrada->mac, espListaCadastro->mac);
        strcpy(espCadastrada->topico, espListaCadastro->topico);
        espCadastrada->prox = NULL;

        if (headListaEspCadastradas == NULL) {
            espCadastrada->teclaRelacionada = 'd';
            headListaEspCadastradas = espCadastrada;
        } else {
            ListaEspCadastradas *temp = headListaEspCadastradas;
            while (temp->prox != NULL) {
                temp = temp->prox;
            }
            temp->prox = espCadastrada;
            espCadastrada->teclaRelacionada = temp->teclaRelacionada + 1;
        }
        removeEspListaCadastro();
        numCadastradas++;

        registrarCadastro(espCadastrada->nomeComodo, espCadastrada->mac);
        confirmaCadastramento(espCadastrada->topico, espCadastrada->nomeComodo, espCadastrada->mac);
    }
}

void adicionaListaCadastro(char *macEsp, char *topico) {
    ListaCadastro *listaCadastro = (ListaCadastro *)malloc(sizeof(ListaCadastro));
    strcpy(listaCadastro->mac, macEsp);
    strcpy(listaCadastro->topico, topico);
    listaCadastro->prox = NULL;
    if (headListaCadastro == NULL) {
        headListaCadastro = listaCadastro;
    } else {
        ListaCadastro *temp = headListaCadastro;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = listaCadastro;
    }

    numCadastro++;
}

void acionaSaidaCliente(char teclaAcionamento) {
    if (headListaEspCadastradas != NULL) {
        ListaEspCadastradas *temp;
        temp = headListaEspCadastradas;
        while (temp != NULL) {
            if (temp->teclaRelacionada == teclaAcionamento) {
                enviaMensagemAcionamentoSaida(temp->topico, temp->mac);
                escreverComandoCliente(temp->nomeComodo, temp->mac, !temp->estado_saida);
                break;
            }
            temp = temp->prox;
        }
    }
}