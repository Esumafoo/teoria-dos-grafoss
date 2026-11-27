#ifndef IO_H
#define IO_H

#include "agm.h"
#include "grafo.h"

int ler_grafo_agm(GrafoListaPonderado **grafo);
void imprimir_resultado_agm(const char *algoritmo, StatusAGM status, const Aresta *agm, size_t num_arestas, int custo_total);
void executar_demonstracao_agm(void);

#endif
