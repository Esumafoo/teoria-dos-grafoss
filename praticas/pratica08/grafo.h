#ifndef GRAFO_H
#define GRAFO_H

#include <stddef.h>

typedef struct ArestaLista {
    int destino;
    int peso;
    struct ArestaLista *prox;
} ArestaLista;

typedef struct {
    size_t num_vertices;
    int direcionado;
    ArestaLista **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_ponderado(size_t num_vertices, int direcionado);
void liberar_grafo_ponderado(GrafoListaPonderado *grafo);
int adicionar_aresta_ponderada(GrafoListaPonderado *grafo, int origem, int destino, int peso);

#endif
