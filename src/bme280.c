#include "bme280.h"

#define NUMERO_VALORES_PARA_MEDIA 10

struct identifier identificador;
Bme280TemperaturaUmidade bme280TemperaturaUmidade = {0.0, 0.0};

struct identifier {
    /* Variável que contém o endereço do dispositivo */
    uint8_t enderecoDispositivo;

    /* Variável que contém o descritor do arquivo */
    int8_t fd;
};

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
    struct identifier identificador;

    identificador = *((struct identifier *)intf_ptr);

    write(identificador.fd, &reg_addr, 1);
    read(identificador.fd, data, len);

    return 0;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
    uint8_t *buf;
    struct identifier identificador;

    identificador = *((struct identifier *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(identificador.fd, buf, len + 1) < (uint16_t)len) {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}

void user_delay_us(uint32_t period, void *intf_ptr) {
    usleep(period);
}

struct bme280_data stream_sensor_data_normal_mode(struct bme280_dev *dev) {
    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;
    dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

    /* Espera 1 segundo para realizar a medição */
    dev->delay_us(100000, dev->intf_ptr);
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
    if (rslt != BME280_OK) {
        fprintf(stderr, "Erro ao transmitir dados do sensor (código: %+d).\n", rslt);
        exit(1);
    }

    return comp_data;
}

void *bme280_defineTemperaturaUmidade() {
    struct bme280_dev dispositivo;
    struct bme280_data comp_data;

    int8_t rslt = BME280_OK;

    /* Interface I2C */
    dispositivo.intf = BME280_I2C_INTF;
    dispositivo.read = user_i2c_read;
    dispositivo.write = user_i2c_write;
    dispositivo.delay_us = user_delay_us;

    /* Ponteiro da interface */
    dispositivo.intf_ptr = &identificador;

    /* Inicializa a bme280 */
    rslt = bme280_init(&dispositivo);
    if (rslt != BME280_OK) {
        printf("erro init\n");
        return NULL;
    }

    while (1) {
        comp_data = stream_sensor_data_normal_mode(&dispositivo);

        float temperaturaMinimaPossivel = 15.00;
        float umidadeMinimaPossivel = 9.00;
        float diferencaMaxima = 1.0;

        /* Verifica se a temperatura ou umidade estão dentro da faixa de valores possíveis.
        *  Se estiverem, verifíca se os valores já foram setados anteriormente.
        *  Se já foram, verifica se a variação está de acordo com a diferença máxima estabelecida. Se estiver, atualiza os valores.
        *  Se não foram, seta o valar obtido.
        */
        if (comp_data.temperature >= temperaturaMinimaPossivel) {
            if (bme280TemperaturaUmidade.temperatura != 0 && (abs(comp_data.temperature - bme280TemperaturaUmidade.temperatura) < diferencaMaxima)) {
                bme280TemperaturaUmidade.temperatura = comp_data.temperature;
            } else if (bme280TemperaturaUmidade.temperatura == 0) {
                bme280TemperaturaUmidade.temperatura = comp_data.temperature;
            }
        }

        if (comp_data.humidity >= umidadeMinimaPossivel) {
            if (bme280TemperaturaUmidade.umidade != 0 && (abs(comp_data.humidity - bme280TemperaturaUmidade.umidade) < diferencaMaxima)) {
                bme280TemperaturaUmidade.umidade = comp_data.humidity;
            } else if (bme280TemperaturaUmidade.umidade == 0) {
                bme280TemperaturaUmidade.umidade = comp_data.humidity;
            }
        }
        sleep(1);
    }
}

void bme280_inicializa() {
    char i2cInterface[] = "/dev/i2c-1";

    if ((identificador.fd = open(i2cInterface, O_RDWR)) < 0) {
        exit(1);
    }

    /* ME280_I2C_ADDR_PRIM : Referente ao endereço 0x76 */
    identificador.enderecoDispositivo = BME280_I2C_ADDR_PRIM;

    if (ioctl(identificador.fd, I2C_SLAVE, identificador.enderecoDispositivo) < 0) {
        exit(1);
    }
}