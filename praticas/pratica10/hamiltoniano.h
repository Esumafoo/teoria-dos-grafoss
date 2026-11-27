#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#include "grafo.h"

int encontrar_caminho_hamiltoniano_bt(const GrafoMatriz *g, int *caminho);
int encontrar_ciclo_hamiltoniano_bt(const GrafoMatriz *g, int *ciclo);

#endif
