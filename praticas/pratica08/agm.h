#ifndef AGM_H
#define AGM_H

#include "grafo.h"

#include <stddef.h>

typedef enum {
    AGM_OK = 1,
    AGM_DESCONEXO = 0,
    AGM_INVALIDO = -1
} StatusAGM;

typedef struct {
    int *pai;
    int *rank;
    int n;
} DSU;

typedef struct {
    int u;
    int v;
    int peso;
} Aresta;

DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);

StatusAGM kruskal(const GrafoListaPonderado *g, Aresta *agm, size_t *num_arestas_agm, int *custo_total);
StatusAGM prim(const GrafoListaPonderado *g, int origem, Aresta *agm, size_t *num_arestas_agm, int *custo_total);

#endif
