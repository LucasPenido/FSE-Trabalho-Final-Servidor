#ifndef INTERFACE_H
#define INTERFACE_H

#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#include "clientes_esp32.h"

// #include "cliente_central.h"
// #include "informacoes_dispositivos.h"
// #include "arquivo.h"

void *menuInformacoes();
void iniciaNcurses();

#endif /* INTERFACE_H */