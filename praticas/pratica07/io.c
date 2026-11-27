#include "io.h"
#include "bellman_ford.h"

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

int ler_entrada_bellman_ford(EntradaBellmanFord *entrada) {
    int n = 0;
    int m = 0;
    int direcionado = 1;
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
            liberar_entrada_bellman_ford(entrada);
            return LEITURA_INVALIDA;
        }
        if (!adicionar_aresta_ponderada(entrada->grafo, u, v, peso)) {
            liberar_entrada_bellman_ford(entrada);
            return LEITURA_INVALIDA;
        }
    }

    if (n > 0) {
        if (scanf("%d %d", &entrada->origem, &entrada->destino) != 2 ||
            entrada->origem < 0 || entrada->origem >= n ||
            entrada->destino < 0 || entrada->destino >= n) {
            liberar_entrada_bellman_ford(entrada);
            return LEITURA_INVALIDA;
        }
    }

    return LEITURA_OK;
}

void liberar_entrada_bellman_ford(EntradaBellmanFord *entrada) {
    if (entrada == NULL) {
        return;
    }
    liberar_grafo_ponderado(entrada->grafo);
    entrada->grafo = NULL;
}

void imprimir_resultado_bellman_ford(const EntradaBellmanFord *entrada, const int *dist, int *pred, int status_ok) {
    size_t i;
    const GrafoListaPonderado *g = entrada->grafo;

    if (!status_ok) {
        reconstruir_ciclo_negativo(g, dist, pred);
        return;
    }

    printf("Distancias minimas (Bellman-Ford) a partir de %d:\n", entrada->origem);
    for (i = 0; i < g->num_vertices; i++) {
        if (dist[i] == DISTANCIA_INFINITA) {
            printf("  vertice %zu: INF\n", i);
        } else {
            printf("  vertice %zu: %d\n", i, dist[i]);
        }
    }
    imprimir_caminho_bellman_ford(entrada->origem, entrada->destino, pred, g->num_vertices);
}

void executar_demonstracao_bellman_ford(void) {
    GrafoListaPonderado *g_valido = criar_grafo_ponderado(5, 1);
    GrafoListaPonderado *g_ciclo = criar_grafo_ponderado(4, 1);
    int dist[5];
    int pred[5];
    EntradaBellmanFord demo;

    if (g_valido != NULL) {
        adicionar_aresta_ponderada(g_valido, 0, 1, 6);
        adicionar_aresta_ponderada(g_valido, 0, 2, 7);
        adicionar_aresta_ponderada(g_valido, 1, 2, 8);
        adicionar_aresta_ponderada(g_valido, 1, 3, 5);
        adicionar_aresta_ponderada(g_valido, 1, 4, -4);
        adicionar_aresta_ponderada(g_valido, 2, 3, -3);
        adicionar_aresta_ponderada(g_valido, 2, 4, 9);
        adicionar_aresta_ponderada(g_valido, 3, 1, -2);
        adicionar_aresta_ponderada(g_valido, 4, 0, 2);
        adicionar_aresta_ponderada(g_valido, 4, 3, 7);

        demo.grafo = g_valido;
        demo.origem = 0;
        demo.destino = 4;
        imprimir_resultado_bellman_ford(&demo, dist, pred, bellman_ford(g_valido, 0, dist, pred));
        liberar_grafo_ponderado(g_valido);
    }

    if (g_ciclo != NULL) {
        adicionar_aresta_ponderada(g_ciclo, 0, 1, 1);
        adicionar_aresta_ponderada(g_ciclo, 1, 2, -2);
        adicionar_aresta_ponderada(g_ciclo, 2, 3, -2);
        adicionar_aresta_ponderada(g_ciclo, 3, 1, -1);

        demo.grafo = g_ciclo;
        demo.origem = 0;
        demo.destino = 3;
        imprimir_resultado_bellman_ford(&demo, dist, pred, bellman_ford(g_ciclo, 0, dist, pred));
        liberar_grafo_ponderado(g_ciclo);
    }
}
