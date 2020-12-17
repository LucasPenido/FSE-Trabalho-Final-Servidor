#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#include "alarme.h"
#include "arquivo.h"
#include "bcm2835.h"
#include "bme280.h"
#include "interface.h"
#include "mqtt.h"

int intruso;

void fechaConexoes() {
    mqtt_disconecta();
    bcm2835_desliga();
    endwin();
}

void trata_interrupcao() {
    exit(0);
}

int main() {
    pthread_t thread_interface, thread_verificaIntruso, thread_bme280;

    bme280_inicializa();
    bcm2835_inicializa();
    mqtt_conecta();

    atexit(fechaConexoes);
    signal(SIGINT, trata_interrupcao);

    iniciaNcurses();
    criaArquivo();

    pthread_create(&thread_interface, NULL, &menuInformacoes, NULL);
    pthread_create(&thread_verificaIntruso, NULL, &verificaIntruso, NULL);
    pthread_create(&thread_bme280, NULL, &bme280_defineTemperaturaUmidade, NULL);

    pthread_join(thread_interface, NULL);

    endwin();

    return 0;
}
