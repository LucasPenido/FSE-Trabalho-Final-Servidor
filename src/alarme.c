#include "alarme.h"

#include "arquivo.h"
#include "bcm2835.h"

void *tocaAlarme() {
    int processoAlarme;

    processoAlarme = fork();

    if (processoAlarme == 0) {
        while (1) {
            // system("omxplayer --no-keys ./Som_Alarme/alarme.mp3 >/dev/null 2>&1");
        }
    } else {
        while (intruso) {
            sleep(1);
        }
        kill(processoAlarme, SIGKILL);
    }

    return NULL;
}

void *verificaIntruso() {
    pthread_t thread_alarme;
    int sensorAtivo = 0;

    while (1) {
        sleep(1);
        if (estadoSensores.estadoSensorPresenca01 || estadoSensores.estadoSensorPresenca02 ||
            estadoSensores.estadoSensorAbertura01 || estadoSensores.estadoSensorAbertura02 ||
            estadoSensores.estadoSensorAbertura03 || estadoSensores.estadoSensorAbertura04) {
            sensorAtivo = 1;
        } else {
            sensorAtivo = 0;
        }

        if (alarme == 1 && sensorAtivo == 1 && intruso == 0) {
            intruso = 1;
            pthread_create(&thread_alarme, NULL, &tocaAlarme, NULL);
            escreverArquivo("Alarme", ALARME, 2);
        } else if (alarme == 0) {
            intruso = 0;
        }
    }
}

// void *tocaAlarme() {
//     while (intruso) {
//         system("omxplayer ./Som_Alarme/alarme2.mp3 >/dev/null 2>&1");
//     }

//     return NULL;
// }
