#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void criaArquivo();
void escreverArquivo(char *tipoAcionamento, int numDispositivo, int estado);
void registrarCadastro(char *nomeComodo, char *mac);
void escreverComandoCliente(char *tipoAcionamento, char *mac, int estado);
#endif