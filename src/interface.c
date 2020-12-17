
#include "interface.h"

#include <pthread.h>

#include "arquivo.h"
#include "bcm2835.h"
#include "bme280.h"
#include "clientes_esp32.h"

#define DISPOSITIVO_DESLIGADO 1
#define DISPOSITIVO_LIGADO 2

EstadoDispositivos estadoDispositivos;
EstadoSensores estadoSensores;
Bme280TemperaturaUmidade bme280TemperaturaUmidade;
ListaCadastro *headListaCadastro;
ListaEspCadastradas *headListaEspCadastradas;

pthread_mutex_t cadastramentoMutex;
int alarme = 0;

void iniciaNcurses() {
    initscr();
    use_default_colors();

    start_color();

    init_pair(DISPOSITIVO_DESLIGADO, COLOR_RED, -1);
    init_pair(DISPOSITIVO_LIGADO, COLOR_GREEN, -1);
    curs_set(0);
    noecho();
}

void printaEstadoDispositivoCliente(WINDOW *menu, int numDispositivo, ListaEspCadastradas *cliente) {
    switch (numDispositivo) {
        case (DISPOSITIVO_ENTRADA):
            if (cliente->estado_entrada == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case (DISPOSITIVO_SAIDA):
            if (cliente->estado_saida == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        default:
            break;
    }
}

void printaEstadoDispositivo(WINDOW *menu, int numDispositivo, int linha) {
    switch (numDispositivo) {
        case LAMPADA_01:
            if (estadoDispositivos.estadoLampada01 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                mvwprintw(menu, linha, 23, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                mvwprintw(menu, linha, 23, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;

        case LAMPADA_02:
            if (estadoDispositivos.estadoLampada02 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                mvwprintw(menu, linha, 23, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                mvwprintw(menu, linha, 23, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case ALARME:
            if (alarme == 1 && intruso == 0) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "Alarme Ativado");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else if (alarme == 0) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "Alarme Desativado");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            } else if (alarme == 1 && intruso == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "Alarme Disparado!!");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case SENSOR_PRESENCA_01:
            if (estadoSensores.estadoSensorPresenca01 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case SENSOR_PRESENCA_02:
            if (estadoSensores.estadoSensorPresenca02 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case SENSOR_ABERTURA_01:
            if (estadoSensores.estadoSensorAbertura01 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case SENSOR_ABERTURA_02:
            if (estadoSensores.estadoSensorAbertura02 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case SENSOR_ABERTURA_03:
            if (estadoSensores.estadoSensorAbertura03 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        case SENSOR_ABERTURA_04:
            if (estadoSensores.estadoSensorAbertura04 == 1) {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
                wprintw(menu, "ON");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_LIGADO));
            } else {
                wattron(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
                wprintw(menu, "OFF");
                wattroff(menu, COLOR_PAIR(DISPOSITIVO_DESLIGADO));
            }
            break;
        default:
            break;
    }
}

void menuCabecalho(WINDOW *cabecalho, int col) {
    char cab[] = "Sistema de Automação Residencial";

    box(cabecalho, 0, 0);
    mvwprintw(cabecalho, 1, (col - strlen(cab)) / 2, "%s", cab);
    wrefresh(cabecalho);
}

void menuNotificacoes(WINDOW *menuNotificacoes, int col) {
    char cabNotificacoes[] = "Notificações";

    werase(menuNotificacoes);
    box(menuNotificacoes, 0, 0);

    mvwprintw(menuNotificacoes, 1, (col - strlen(cabNotificacoes)) / 2, "%s", cabNotificacoes);
    mvwhline(menuNotificacoes, 2, 1, 0, col - 2);

    if (numCadastro > 0) {
        mvwprintw(menuNotificacoes, 3, 1, "%d Dispositivo(s) para cadastrar", numCadastro);
        mvwhline(menuNotificacoes, 4, 1, 0, col - 2);
        mvwprintw(menuNotificacoes, 5, 1, "c - Cadastrar novo dispositivo");
    }

    wrefresh(menuNotificacoes);
}

void menuSensores(WINDOW *menuSensores, int col) {
    char cabSensores[] = "Quadro de Sensores";

    werase(menuSensores);
    box(menuSensores, 0, 0);

    mvwprintw(menuSensores, 1, (col - strlen(cabSensores)) / 2, "%s", cabSensores);
    mvwhline(menuSensores, 2, 1, 0, col - 2);

    mvwprintw(menuSensores, 3, 1, "Sensor Presenca Sala: ");
    printaEstadoDispositivo(menuSensores, SENSOR_PRESENCA_01, 3);

    mvwprintw(menuSensores, 4, 1, "Sensor Presenca Cozinha: ");
    printaEstadoDispositivo(menuSensores, SENSOR_PRESENCA_02, 4);

    mvwprintw(menuSensores, 5, 1, "Sensor Porta Cozinha: ");
    printaEstadoDispositivo(menuSensores, SENSOR_ABERTURA_01, 5);

    mvwprintw(menuSensores, 6, 1, "Sensor Janela Cozinha: ");
    printaEstadoDispositivo(menuSensores, SENSOR_ABERTURA_02, 6);

    mvwprintw(menuSensores, 7, 1, "Sensor Porta Sala: ");
    printaEstadoDispositivo(menuSensores, SENSOR_ABERTURA_03, 7);

    mvwprintw(menuSensores, 8, 1, "Sensor Janela Sala: ");
    printaEstadoDispositivo(menuSensores, SENSOR_ABERTURA_04, 8);

    mvwhline(menuSensores, 13, 1, 0, col - 2);

    if (alarme == 1) {
        mvwprintw(menuSensores, 14, 1, "7 - Desativar Alarme: ");
    } else {
        mvwprintw(menuSensores, 14, 1, "7 - Ativar Alarme: ");
    }
    printaEstadoDispositivo(menuSensores, ALARME, 13);

    wrefresh(menuSensores);
}

void menuDispositivos(WINDOW *menu, int col) {
    char cabDispositivos[] = "Quadro de Dispositivos";

    bcm2835_verificaDispositivos();
    bcm2835_verificaSensores();

    werase(menu);
    box(menu, 0, 0);

    mvwprintw(menu, 1, (col - strlen(cabDispositivos)) / 2, "%s", cabDispositivos);
    mvwhline(menu, 2, 1, 0, col - 2);

    mvwprintw(menu, 3, 1, "1 - Lâmpada Cozinha: ");
    printaEstadoDispositivo(menu, LAMPADA_01, 3);

    mvwprintw(menu, 4, 1, "2 - Lâmpada Sala: ");
    printaEstadoDispositivo(menu, LAMPADA_02, 4);

    mvwprintw(menu, 6, 1, "Temperatura: %.2f ºC", bme280TemperaturaUmidade.temperatura);
    mvwprintw(menu, 7, 1, "Umidade: %.2f %%", bme280TemperaturaUmidade.umidade);
    wrefresh(menu);
}

void imprimeCliente(ListaEspCadastradas *cliente, int tamLin, int tamCol, int linInicio, int colInicio) {
    WINDOW *menuCliente;
    menuCliente = newwin(tamLin, tamCol, linInicio, colInicio);
    werase(menuCliente);
    box(menuCliente, 0, 0);

    mvwprintw(menuCliente, 1, (tamCol - strlen(cliente->nomeComodo)) / 2, "%s", cliente->nomeComodo);
    mvwprintw(menuCliente, 3, 1, "Entrada: ");
    printaEstadoDispositivoCliente(menuCliente, DISPOSITIVO_ENTRADA, cliente);
    mvwprintw(menuCliente, 4, 1, "%c - Saida: ", cliente->teclaRelacionada);
    printaEstadoDispositivoCliente(menuCliente, DISPOSITIVO_SAIDA, cliente);
    mvwprintw(menuCliente, 5, 1, "Temp.: %.2f ºC", cliente->temperatura);
    mvwprintw(menuCliente, 6, 1, "Umi.: %.2f %%", cliente->umidade);
    wrefresh(menuCliente);
    delwin(menuCliente);
}

void menusClientes(int linInicio, int colInicio) {
    int tamColClientes, colInicioOriginal;
    colInicioOriginal = colInicio;

    tamColClientes = (getmaxx(stdscr) - colInicio) / 3;

    if (headListaEspCadastradas != NULL) {
        ListaEspCadastradas *temp;
        temp = headListaEspCadastradas;
        while (temp != NULL) {
            imprimeCliente(temp, 8, tamColClientes, linInicio, colInicio);
            colInicio += tamColClientes;
            if ((colInicio + tamColClientes) >= getmaxx(stdscr)) {
                linInicio += 8;
                colInicio = colInicioOriginal;
            }
            temp = temp->prox;
        }
    }
}

void menuCadastro() {
    refresh();
    WINDOW *menuCadastro, *cabecalhoCadastro;
    int terminalnCol, op = 0;
    char nomeComodo[50];

    terminalnCol = getmaxx(stdscr);

    cabecalhoCadastro = newwin(3, terminalnCol, 0, 0);

    char cab[] = "Menu de Cadastramento de Cliente";

    box(cabecalhoCadastro, 0, 0);
    mvwprintw(cabecalhoCadastro, 1, (terminalnCol - strlen(cab)) / 2, "%s", cab);
    wrefresh(cabecalhoCadastro);

    menuCadastro = newwin(6, 26, 4, 0);
    mvwprintw(menuCadastro, 0, 0, "MAC: %s", headListaCadastro->mac);

    mvwprintw(menuCadastro, 1, 0, "1 - Cadastrar");
    mvwprintw(menuCadastro, 2, 0, "2 - Rejeitar");

    wrefresh(menuCadastro);

    while (op != '1' && op != '2') {
        op = wgetch(menuCadastro);

        switch (op) {
            case '1':
                curs_set(1);
                echo();
                mvwprintw(menuCadastro, 3, 0, "Digite o nome do cômodo: ");
                wmove(menuCadastro, 4, 0);
                wgetstr(menuCadastro, nomeComodo);
                cadastraEsp(nomeComodo);
                break;
            case '2':
                removeEspListaCadastro();
                break;
            default:
                break;
        }
    }
    curs_set(0);
    noecho();
    delwin(menuCadastro);
    delwin(cabecalhoCadastro);
    refresh();
}

void leTeclado(WINDOW *dispositivos) {
    int op;
    if ((op = wgetch(dispositivos)) != ERR) {
        switch (op) {
            case '0':
                exit(1);
                break;
            case '1':
                escreverArquivo("Dispositivo", LAMPADA_01, !estadoDispositivos.estadoLampada01);
                bcm2835_acionarDispositivo(LAMPADA_01, !estadoDispositivos.estadoLampada01);
                break;
            case '2':
                escreverArquivo("Dispositivo", LAMPADA_02, !estadoDispositivos.estadoLampada02);
                bcm2835_acionarDispositivo(LAMPADA_02, !estadoDispositivos.estadoLampada02);
                break;
            case '7':
                alarme = !alarme;
                escreverArquivo("Alarme", ALARME, alarme);
                break;
            case 'c':
                if (headListaCadastro != NULL) {
                    pthread_mutex_lock(&cadastramentoMutex);
                    menuCadastro();
                    pthread_mutex_unlock(&cadastramentoMutex);
                }
                break;
            default:
                // Para os casos de acionamento das saídas dos clientes
                if (op >= 'd' && op <= 'z') {
                    acionaSaidaCliente(op);
                }
                break;
        }
    }
}

void *menuInformacoes() {
    int terminalnCol, sensoresnCol, dispositivosnCol, dispositivosnLin, notificacoesnCol;
    WINDOW *dispositivos, *cabecalho, *sensores, *notificacoes;

    pthread_mutex_init(&cadastramentoMutex, NULL);

    terminalnCol = getmaxx(stdscr);

    cabecalho = newwin(3, terminalnCol, 0, 0);

    dispositivos = newwin(9, terminalnCol / 4, 4, 0);
    dispositivosnCol = getmaxx(dispositivos);
    dispositivosnLin = getmaxy(dispositivos);

    nodelay(dispositivos, TRUE);

    sensores = newwin(16, terminalnCol / 4 + 5, 4, dispositivosnCol);
    sensoresnCol = getmaxx(sensores);

    notificacoes = newwin(7, terminalnCol / 4, dispositivosnLin + 4, 0);
    notificacoesnCol = getmaxx(notificacoes);

    while (1) {
        pthread_mutex_lock(&cadastramentoMutex);
        menuCabecalho(cabecalho, terminalnCol);
        menuDispositivos(dispositivos, dispositivosnCol);
        menuNotificacoes(notificacoes, notificacoesnCol);
        menuSensores(sensores, sensoresnCol);
        menusClientes(4, terminalnCol / 2 + 5);
        pthread_mutex_unlock(&cadastramentoMutex);

        leTeclado(dispositivos);

        usleep(100000);
    }
}