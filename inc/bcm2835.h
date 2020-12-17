#ifndef BCM28352_H
#define BCM28352_H

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "alarme.h"
#include "bcm2835Driver.h"

#define LAMPADA_01 RPI_V2_GPIO_P1_11          // Lâmpada Cozinha
#define LAMPADA_02 RPI_V2_GPIO_P1_12          // Lâmpada Sala
#define SENSOR_PRESENCA_01 RPI_V2_GPIO_P1_22  // Sensor Presensa Sala
#define SENSOR_PRESENCA_02 RPI_V2_GPIO_P1_37  // Sensor Presensa Cozinha
#define SENSOR_ABERTURA_01 RPI_V2_GPIO_P1_29  // Sensor Abertura Porta Cozinha
#define SENSOR_ABERTURA_02 RPI_V2_GPIO_P1_31  // Sensor Abertura Janela Cozinha
#define SENSOR_ABERTURA_03 RPI_V2_GPIO_P1_32  // Sensor Abertura Porta Sala
#define SENSOR_ABERTURA_04 RPI_V2_GPIO_P1_36  // Sensor Abertura Janela Sala
#define ALARME 7
#define NUM_SENSORES 6

typedef struct
{
    unsigned int estadoLampada01 : 1;
    unsigned int estadoLampada02 : 1;
} EstadoDispositivos;

typedef struct {
    unsigned int estadoSensorPresenca01 : 1;
    unsigned int estadoSensorPresenca02 : 1;
    unsigned int estadoSensorAbertura01 : 1;
    unsigned int estadoSensorAbertura02 : 1;
    unsigned int estadoSensorAbertura03 : 1;
    unsigned int estadoSensorAbertura04 : 1;
} EstadoSensores;

extern EstadoSensores estadoSensores;
extern EstadoDispositivos estadoDispositivos;

void bcm2835_acionarDispositivo(int numDispositivo, int estado);
void bcm2835_verificaDispositivos();
void bcm2835_verificaSensores();
void bcm2835_inicializa();
void bcm2835_desliga();

#endif /* BCM2835_H */
