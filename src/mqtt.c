#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MQTTAsync.h"
#include "cJSON.h"
#include "clientes_esp32.h"

#define ADDRESS "tcp://test.mosquitto.org:1883"
#define CLIENTID "ExampleClientSub"
#define TOPICOS_DISPOSITIVOS "fse2020/160013143/dispositivos/#"
#define TOPICOS_TEMPERATURA "fse2020/160013143/+/temperatura"
#define TOPICOS_UMIDADE "fse2020/160013143/+/umidade"
#define TOPICOS_ESTADOS "fse2020/160013143/+/estado"
#define QOS 1
#define TIMEOUT 10000L

#define CADASTRAR 0
#define CONFIRMA_CADASTRO 1
#define TROCA_ESTADO_SAIDA 2

MQTTAsync client;
volatile MQTTAsync_token deliveredtoken;
int disc_finished = 0;
int subscribed = 0;
int finished = 0;

void enviaMqqt(char* mensagem, char* topico) {
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    int rc;

    opts.context = client;
    pubmsg.payload = mensagem;
    pubmsg.payloadlen = strlen(mensagem);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    deliveredtoken = 0;
    if ((rc = MQTTAsync_sendMessage(client, topico, &pubmsg, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start sendMessage, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void enviaMensagemAcionamentoSaida(char* topico, char* mac) {
    cJSON* root;

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "tipo", TROCA_ESTADO_SAIDA);
    cJSON_AddStringToObject(root, "mac", mac);
    char* json = cJSON_Print(root);

    enviaMqqt(json, topico);
}

void confirmaCadastramento(char* topico, char* nomeComodo, char* mac) {
    cJSON* root;

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "tipo", CONFIRMA_CADASTRO);
    cJSON_AddStringToObject(root, "mac", mac);
    cJSON_AddStringToObject(root, "comodo", nomeComodo);
    char* json = cJSON_Print(root);

    enviaMqqt(json, topico);
}

void trataMensagem(char* mensagem, char* topicName, void* context) {
    cJSON* mqttMensagem = cJSON_Parse(mensagem);
    char* macEsp = cJSON_GetObjectItem(mqttMensagem, "mac")->valuestring;

    if (strstr(topicName, "temperatura")) {
        float temperatura = cJSON_GetObjectItem(mqttMensagem, "temperatura")->valuedouble;
        adicionaTemperaturaCliente(temperatura, macEsp);
    } else if (strstr(topicName, "umidade")) {
        float umidade = cJSON_GetObjectItem(mqttMensagem, "umidade")->valuedouble;
        adicionaUmidadeCliente(umidade, macEsp);
    } else if (strstr(topicName, "estado")) {
        float estadoEntrada = cJSON_GetObjectItem(mqttMensagem, "estado_entrada")->valueint;
        float estadoSaida = cJSON_GetObjectItem(mqttMensagem, "estado_saida")->valueint;
        adicionaEstadosCliente(estadoEntrada, estadoSaida, macEsp);
    } else {
        int tipo = cJSON_GetObjectItem(mqttMensagem, "tipo")->valueint;

        if (tipo == CADASTRAR) {
            adicionaListaCadastro(macEsp, topicName);
        }
    }
}

int msgarrvd(void* context, char* topicName, int topicLen, MQTTAsync_message* message) {
    trataMensagem(message->payload, topicName, context);

    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}
void onDisconnect(void* context, MQTTAsync_successData* response) {
    disc_finished = 1;
}
void onSubscribe(void* context, MQTTAsync_successData* response) {
    subscribed = 1;
}
void onSubscribeFailure(void* context, MQTTAsync_failureData* response) {
    printf("Subscribe failed, rc %d\n", response ? response->code : 0);
    finished = 1;
}
void onConnectFailure(void* context, MQTTAsync_failureData* response) {
    printf("Connect failed, rc %d\n", response ? response->code : 0);
    finished = 1;
}
void onConnect(void* context, MQTTAsync_successData* response) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;
    opts.onSuccess = onSubscribe;
    opts.onFailure = onSubscribeFailure;
    opts.context = client;
    deliveredtoken = 0;
    if ((rc = MQTTAsync_subscribe(client, TOPICOS_DISPOSITIVOS, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start subscribe, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    if ((rc = MQTTAsync_subscribe(client, TOPICOS_TEMPERATURA, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start subscribe, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    if ((rc = MQTTAsync_subscribe(client, TOPICOS_UMIDADE, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start subscribe, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    if ((rc = MQTTAsync_subscribe(client, TOPICOS_ESTADOS, QOS, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start subscribe, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void mqtt_disconecta() {
    MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    disc_opts.onSuccess = onDisconnect;
    if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start disconnect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    while (!disc_finished) {
        usleep(10000L);
    }

    MQTTAsync_destroy(&client);
}

void connlost(void* context, char* cause) {
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;
    printf("Connection lost");
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        finished = 1;
    }
}

void* mqtt_conecta() {
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;
    MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    while (!subscribed) {
        usleep(10000L);
    }

    return NULL;
}