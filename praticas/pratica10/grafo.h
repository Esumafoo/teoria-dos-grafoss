#ifndef GRAFO_H
#define GRAFO_H

#include <limits.h>
#include <stddef.h>

#define SEM_ARESTA INT_MAX

typedef struct {
    size_t num_vertices;
    int direcionado;
    int **peso;
} GrafoMatriz;

GrafoMatriz *criar_grafo_matriz(size_t num_vertices, int direcionado);
void liberar_grafo_matriz(GrafoMatriz *grafo);
int adicionar_aresta_matriz(GrafoMatriz *grafo, int u, int v, int peso);
int possui_aresta_matriz(const GrafoMatriz *grafo, int u, int v);

#endif
