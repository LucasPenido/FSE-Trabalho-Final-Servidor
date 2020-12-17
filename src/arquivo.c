#include "arquivo.h"

void criaArquivo() {
    FILE *file = fopen("dados_acionamento.csv", "w");
    fprintf(file, "DataHora, Tipo Acionamento, Numero Dispositivo, Estado\n");
    fclose(file);
}

void defineDataHoraAtual(char **dataHora) {
    char cur_time[128];
    *dataHora = malloc(128);

    time_t t;
    struct tm *ptm;

    t = time(NULL);
    ptm = localtime(&t);

    strftime(cur_time, 128, "%d/%m/%Y %H:%M:%S", ptm);

    strcpy(*dataHora, cur_time);
}

void registrarCadastro(char *nomeComodo, char *mac) {
    FILE *file;
    char *dataHora;

    file = fopen("dados_acionamento.csv", "a");
    defineDataHoraAtual(&dataHora);
    fprintf(file, "%s,%s,%s,%s\r\n", dataHora, "Cadastramento", nomeComodo, mac);
    fclose(file);
}

void escreverComandoCliente(char *tipoAcionamento, char *mac, int estado) {
    FILE *file;
    char *dataHora, charEstado[11];

    if (estado == 0) {
        strcpy(charEstado, "Desacionou");
    } else if (estado == 1) {
        strcpy(charEstado, "Acionou");
    }

    file = fopen("dados_acionamento.csv", "a");
    defineDataHoraAtual(&dataHora);
    fprintf(file, "%s,%s,%s,%s\r\n", dataHora, tipoAcionamento, mac, charEstado);
    fclose(file);
}

void escreverArquivo(char *tipoAcionamento, int numDispositivo, int estado) {
    FILE *file;
    char *dataHora, charEstado[11];

    if (estado == 0) {
        strcpy(charEstado, "Desacionou");
    } else if (estado == 1) {
        strcpy(charEstado, "Acionou");
    } else if (estado == 2) {
        strcpy(charEstado, "Disparado");
    }
    file = fopen("dados_acionamento.csv", "a");
    defineDataHoraAtual(&dataHora);
    fprintf(file, "%s,%s,%d,%s\r\n", dataHora, tipoAcionamento, numDispositivo, charEstado);
    fclose(file);
}