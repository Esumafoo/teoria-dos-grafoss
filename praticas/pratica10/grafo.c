#include "grafo.h"

#include <stdlib.h>

static int vertice_valido(const GrafoMatriz *grafo, int vertice) {
    if (grafo == NULL || vertice < 0) {
        return 0;
    }
    return (size_t)vertice < grafo->num_vertices;
}

GrafoMatriz *criar_grafo_matriz(size_t num_vertices, int direcionado) {
    GrafoMatriz *grafo = malloc(sizeof(GrafoMatriz));
    size_t i;
    size_t j;

    if (grafo == NULL) {
        return NULL;
    }

    grafo->num_vertices = num_vertices;
    grafo->direcionado = direcionado ? 1 : 0;
    grafo->peso = NULL;

    if (num_vertices == 0) {
        return grafo;
    }

    grafo->peso = calloc(num_vertices, sizeof(int *));
    if (grafo->peso == NULL) {
        free(grafo);
        return NULL;
    }

    for (i = 0; i < num_vertices; i++) {
        grafo->peso[i] = malloc(num_vertices * sizeof(int));
        if (grafo->peso[i] == NULL) {
            liberar_grafo_matriz(grafo);
            return NULL;
        }
        for (j = 0; j < num_vertices; j++) {
            grafo->peso[i][j] = SEM_ARESTA;
        }
    }

    return grafo;
}

void liberar_grafo_matriz(GrafoMatriz *grafo) {
    size_t i;

    if (grafo == NULL) {
        return;
    }

    if (grafo->peso != NULL) {
        for (i = 0; i < grafo->num_vertices; i++) {
            free(grafo->peso[i]);
        }
        free(grafo->peso);
    }

    free(grafo);
}

int adicionar_aresta_matriz(GrafoMatriz *grafo, int u, int v, int peso) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v) || peso == SEM_ARESTA) {
        return 0;
    }

    grafo->peso[u][v] = peso;
    if (!grafo->direcionado) {
        grafo->peso[v][u] = peso;
    }
    return 1;
}

int possui_aresta_matriz(const GrafoMatriz *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v)) {
        return 0;
    }
    return grafo->peso[u][v] != SEM_ARESTA;
}
