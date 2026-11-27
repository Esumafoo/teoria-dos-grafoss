#ifndef COBERTURA_H
#define COBERTURA_H

#include "grafo.h"

#include <stddef.h>

int eh_cobertura(const GrafoLista *g, const int *selecionado);
int cobertura_minima_bt(const GrafoLista *g, int *melhor_selecao, size_t *tamanho_minimo);
int cobertura_gulosa(const GrafoLista *g, int *selecionado, size_t *tamanho);

#endif
