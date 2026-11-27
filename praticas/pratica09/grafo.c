#include "grafo.h"

#include <stdlib.h>

static int vertice_valido(const GrafoLista *grafo, int vertice) {
    if (grafo == NULL || vertice < 0) {
        return 0;
    }
    return (size_t)vertice < grafo->num_vertices;
}

static int inserir_no(GrafoLista *grafo, int origem, int destino) {
    NoLista *novo = malloc(sizeof(NoLista));
    if (novo == NULL) {
        return 0;
    }
    novo->destino = destino;
    novo->prox = grafo->adj[origem];
    grafo->adj[origem] = novo;
    return 1;
}

GrafoLista *criar_grafo(size_t num_vertices) {
    GrafoLista *grafo = malloc(sizeof(GrafoLista));
    if (grafo == NULL) {
        return NULL;
    }
    grafo->num_vertices = num_vertices;
    grafo->adj = NULL;

    if (num_vertices == 0) {
        return grafo;
    }

    grafo->adj = calloc(num_vertices, sizeof(NoLista *));
    if (grafo->adj == NULL) {
        free(grafo);
        return NULL;
    }

    return grafo;
}

void liberar_grafo(GrafoLista *grafo) {
    size_t i;

    if (grafo == NULL) {
        return;
    }

    if (grafo->adj != NULL) {
        for (i = 0; i < grafo->num_vertices; i++) {
            NoLista *atual = grafo->adj[i];
            while (atual != NULL) {
                NoLista *proximo = atual->prox;
                free(atual);
                atual = proximo;
            }
        }
        free(grafo->adj);
    }

    free(grafo);
}

int sao_adjacentes(const GrafoLista *grafo, int u, int v) {
    const NoLista *atual;

    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v)) {
        return 0;
    }

    atual = grafo->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

int adicionar_aresta(GrafoLista *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v)) {
        return 0;
    }
    if (sao_adjacentes(grafo, u, v)) {
        return 1;
    }
    if (!inserir_no(grafo, u, v)) {
        return 0;
    }
    if (u != v && !inserir_no(grafo, v, u)) {
        NoLista *removido = grafo->adj[u];
        grafo->adj[u] = removido->prox;
        free(removido);
        return 0;
    }
    return 1;
}
