#include "io.h"
#include "dijkstra.h"

#include <stdio.h>
#include <stdlib.h>

enum {
    LEITURA_OK = 1,
    LEITURA_VAZIA = 0,
    LEITURA_INVALIDA = -1
};

static int ler_cabecalho(int *n, int *m, int *direcionado) {
    int lidos = scanf("%d %d %d", n, m, direcionado);
    if (lidos == EOF) {
        return LEITURA_VAZIA;
    }
    if (lidos != 3 || *n < 0 || *m < 0 || (*direcionado != 0 && *direcionado != 1)) {
        return LEITURA_INVALIDA;
    }
    return LEITURA_OK;
}

int ler_entrada_dijkstra(EntradaDijkstra *entrada) {
    int n = 0;
    int m = 0;
    int direcionado = 0;
    int i;
    int status;

    if (entrada == NULL) {
        return LEITURA_INVALIDA;
    }
    entrada->grafo = NULL;
    entrada->origem = 0;
    entrada->destino = 0;

    status = ler_cabecalho(&n, &m, &direcionado);
    if (status != LEITURA_OK) {
        return status;
    }

    entrada->grafo = criar_grafo_ponderado((size_t)n, direcionado);
    if (entrada->grafo == NULL) {
        return LEITURA_INVALIDA;
    }

    for (i = 0; i < m; i++) {
        int u;
        int v;
        int peso;
        if (scanf("%d %d %d", &u, &v, &peso) != 3) {
            liberar_entrada_dijkstra(entrada);
            return LEITURA_INVALIDA;
        }
        if (peso < 0 || !adicionar_aresta_ponderada(entrada->grafo, u, v, peso)) {
            liberar_entrada_dijkstra(entrada);
            return LEITURA_INVALIDA;
        }
    }

    if (n > 0) {
        if (scanf("%d %d", &entrada->origem, &entrada->destino) != 2 ||
            entrada->origem < 0 || entrada->origem >= n ||
            entrada->destino < 0 || entrada->destino >= n) {
            liberar_entrada_dijkstra(entrada);
            return LEITURA_INVALIDA;
        }
    }

    return LEITURA_OK;
}

void liberar_entrada_dijkstra(EntradaDijkstra *entrada) {
    if (entrada == NULL) {
        return;
    }
    liberar_grafo_ponderado(entrada->grafo);
    entrada->grafo = NULL;
}

void imprimir_resultado_dijkstra(const EntradaDijkstra *entrada, const int *dist, const int *pred) {
    size_t i;
    const GrafoListaPonderado *g = entrada->grafo;

    printf("Distancias minimas a partir do vertice %d:\n", entrada->origem);
    for (i = 0; i < g->num_vertices; i++) {
        if (dist[i] == DISTANCIA_INFINITA) {
            printf("  vertice %zu: INF\n", i);
        } else {
            printf("  vertice %zu: %d\n", i, dist[i]);
        }
    }

    reconstruir_caminho(entrada->origem, entrada->destino, pred);
}

void executar_demonstracao_dijkstra(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(6, 0);
    int dist[6];
    int pred[6];
    EntradaDijkstra demo;

    if (g == NULL) {
        fprintf(stderr, "Erro ao alocar grafo de demonstracao.\n");
        return;
    }

    adicionar_aresta_ponderada(g, 0, 1, 7);
    adicionar_aresta_ponderada(g, 0, 2, 9);
    adicionar_aresta_ponderada(g, 0, 5, 14);
    adicionar_aresta_ponderada(g, 1, 2, 10);
    adicionar_aresta_ponderada(g, 1, 3, 15);
    adicionar_aresta_ponderada(g, 2, 3, 11);
    adicionar_aresta_ponderada(g, 2, 5, 2);
    adicionar_aresta_ponderada(g, 3, 4, 6);
    adicionar_aresta_ponderada(g, 4, 5, 9);

    demo.grafo = g;
    demo.origem = 0;
    demo.destino = 4;

    if (dijkstra(g, demo.origem, dist, pred)) {
        imprimir_resultado_dijkstra(&demo, dist, pred);
    }

    liberar_grafo_ponderado(g);
}
