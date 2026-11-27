#ifndef GRAFO_H
#define GRAFO_H

#include <stddef.h>

typedef struct NoLista {
    int destino;
    struct NoLista *prox;
} NoLista;

typedef struct {
    size_t num_vertices;
    NoLista **adj;
} GrafoLista;

GrafoLista *criar_grafo(size_t num_vertices);
void liberar_grafo(GrafoLista *grafo);
int adicionar_aresta(GrafoLista *grafo, int u, int v);
int sao_adjacentes(const GrafoLista *grafo, int u, int v);

#endif
