#ifndef ALARME_H
#define ALARME_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void *tocaAlarme();
void *verificaIntruso();

extern int alarme;
extern int intruso;

#endif