#ifndef BME280_H
#define BME280_H

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "bme280Driver.h"

typedef struct {
    float temperatura;
    float umidade;
} Bme280TemperaturaUmidade;

extern Bme280TemperaturaUmidade bme280TemperaturaUmidade;

void *bme280_defineTemperaturaUmidade();
void bme280_inicializa();

#endif /* BME280_H_ */