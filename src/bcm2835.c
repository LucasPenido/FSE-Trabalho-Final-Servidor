#include "bcm2835.h"

void configura_pinos() {
    bcm2835_gpio_fsel(LAMPADA_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMPADA_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(SENSOR_PRESENCA_01, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_PRESENCA_02, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_01, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_02, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_03, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(SENSOR_ABERTURA_04, BCM2835_GPIO_FSEL_INPT);
}

void bcm2835_acionarDispositivo(int numDispositivo, int estado) {
    bcm2835_gpio_write(numDispositivo, estado);
}

void bcm2835_verificaDispositivos() {
    estadoDispositivos.estadoLampada01 = bcm2835_gpio_lev(LAMPADA_01);
    estadoDispositivos.estadoLampada02 = bcm2835_gpio_lev(LAMPADA_02);
}

void bcm2835_verificaSensores() {
    estadoSensores.estadoSensorPresenca01 = bcm2835_gpio_lev(SENSOR_PRESENCA_01);
    estadoSensores.estadoSensorPresenca02 = bcm2835_gpio_lev(SENSOR_PRESENCA_02);
    estadoSensores.estadoSensorAbertura01 = bcm2835_gpio_lev(SENSOR_ABERTURA_01);
    estadoSensores.estadoSensorAbertura02 = bcm2835_gpio_lev(SENSOR_ABERTURA_02);
    estadoSensores.estadoSensorAbertura03 = bcm2835_gpio_lev(SENSOR_ABERTURA_03);
    estadoSensores.estadoSensorAbertura04 = bcm2835_gpio_lev(SENSOR_ABERTURA_04);
}

void bcm2835_desliga() {
    bcm2835_close();
}

void bcm2835_inicializa() {
    const struct sched_param priority = {1};
    sched_setscheduler(0, SCHED_FIFO, &priority);

    mlockall(MCL_CURRENT | MCL_FUTURE);
    if (!bcm2835_init()) {
        exit(1);
    }

    configura_pinos();
}