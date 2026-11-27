#include "grafo.h"

#include <stdlib.h>

static int vertice_valido(const GrafoListaPonderado *grafo, int vertice) {
    if (grafo == NULL || vertice < 0) {
        return 0;
    }
    return (size_t)vertice < grafo->num_vertices;
}

static int inserir_arco(GrafoListaPonderado *grafo, int origem, int destino, int peso) {
    ArestaLista *novo_no = malloc(sizeof(ArestaLista));
    if (novo_no == NULL) {
        return 0;
    }
    novo_no->destino = destino;
    novo_no->peso = peso;
    novo_no->prox = grafo->adj[origem];
    grafo->adj[origem] = novo_no;
    return 1;
}

GrafoListaPonderado *criar_grafo_ponderado(size_t num_vertices, int direcionado) {
    GrafoListaPonderado *grafo = malloc(sizeof(GrafoListaPonderado));
    if (grafo == NULL) {
        return NULL;
    }
    grafo->num_vertices = num_vertices;
    grafo->direcionado = direcionado ? 1 : 0;
    grafo->adj = NULL;

    if (num_vertices == 0) {
        return grafo;
    }

    grafo->adj = calloc(num_vertices, sizeof(ArestaLista *));
    if (grafo->adj == NULL) {
        free(grafo);
        return NULL;
    }

    return grafo;
}

void liberar_grafo_ponderado(GrafoListaPonderado *grafo) {
    size_t i;

    if (grafo == NULL) {
        return;
    }

    if (grafo->adj != NULL) {
        for (i = 0; i < grafo->num_vertices; i++) {
            ArestaLista *atual = grafo->adj[i];
            while (atual != NULL) {
                ArestaLista *proximo = atual->prox;
                free(atual);
                atual = proximo;
            }
        }
        free(grafo->adj);
    }

    free(grafo);
}

int adicionar_aresta_ponderada(GrafoListaPonderado *grafo, int origem, int destino, int peso) {
    if (!vertice_valido(grafo, origem) || !vertice_valido(grafo, destino)) {
        return 0;
    }

    if (!inserir_arco(grafo, origem, destino, peso)) {
        return 0;
    }

    if (!grafo->direcionado && origem != destino) {
        if (!inserir_arco(grafo, destino, origem, peso)) {
            ArestaLista *removido = grafo->adj[origem];
            grafo->adj[origem] = removido->prox;
            free(removido);
            return 0;
        }
    }

    return 1;
}
