#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "grafo.h"

#include <limits.h>
#include <stddef.h>

#define DISTANCIA_INFINITA INT_MAX
#define SEM_PREDECESSOR (-1)

typedef enum {
    BF_STATUS_OK = 1,
    BF_STATUS_CICLO_NEGATIVO = 0,
    BF_STATUS_ENTRADA_INVALIDA = -1
} StatusBellmanFord;

StatusBellmanFord bellman_ford_status(const GrafoListaPonderado *g, int origem, int *dist, int *pred);
int bellman_ford(const GrafoListaPonderado *g, int origem, int *dist, int *pred);
int obter_ciclo_negativo(const GrafoListaPonderado *g, const int *dist, int *pred, int *ciclo, size_t *tamanho);
void reconstruir_ciclo_negativo(const GrafoListaPonderado *g, const int *dist, int *pred);
int obter_caminho_bellman_ford(int origem, int destino, const int *pred, size_t num_vertices, int *caminho, size_t *tamanho);
void imprimir_caminho_bellman_ford(int origem, int destino, const int *pred, size_t num_vertices);

#endif
