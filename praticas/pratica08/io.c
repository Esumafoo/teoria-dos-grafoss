#include "io.h"

#include <stdio.h>
#include <stdlib.h>

enum {
    LEITURA_OK = 1,
    LEITURA_VAZIA = 0,
    LEITURA_INVALIDA = -1
};

int ler_grafo_agm(GrafoListaPonderado **grafo) {
    int n = 0;
    int m = 0;
    int i;
    int lidos;

    if (grafo == NULL) {
        return LEITURA_INVALIDA;
    }
    *grafo = NULL;

    lidos = scanf("%d %d", &n, &m);
    if (lidos == EOF) {
        return LEITURA_VAZIA;
    }
    if (lidos != 2 || n < 0 || m < 0) {
        return LEITURA_INVALIDA;
    }

    *grafo = criar_grafo_ponderado((size_t)n, 0);
    if (*grafo == NULL) {
        return LEITURA_INVALIDA;
    }

    for (i = 0; i < m; i++) {
        int u;
        int v;
        int peso;
        if (scanf("%d %d %d", &u, &v, &peso) != 3 ||
            !adicionar_aresta_ponderada(*grafo, u, v, peso)) {
            liberar_grafo_ponderado(*grafo);
            *grafo = NULL;
            return LEITURA_INVALIDA;
        }
    }

    return LEITURA_OK;
}

void imprimir_resultado_agm(const char *algoritmo, StatusAGM status, const Aresta *agm, size_t num_arestas, int custo_total) {
    size_t i;

    if (status == AGM_INVALIDO) {
        printf("%s: entrada invalida.\n", algoritmo);
        return;
    }

    printf("%s (%s): custo = %d, arestas = %zu\n",
           algoritmo,
           status == AGM_OK ? "arvore geradora minima" : "grafo desconexo - floresta/componente",
           custo_total,
           num_arestas);

    for (i = 0; i < num_arestas; i++) {
        printf("  (%d, %d) peso %d\n", agm[i].u, agm[i].v, agm[i].peso);
    }
}

void executar_demonstracao_agm(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(6, 0);
    Aresta agm_kruskal[5];
    Aresta agm_prim[5];
    size_t qtd_kruskal = 0;
    size_t qtd_prim = 0;
    int custo_kruskal = 0;
    int custo_prim = 0;
    StatusAGM st_kruskal;
    StatusAGM st_prim;

    if (g == NULL) {
        fprintf(stderr, "Erro ao criar grafo de demonstracao.\n");
        return;
    }

    adicionar_aresta_ponderada(g, 0, 1, 4);
    adicionar_aresta_ponderada(g, 0, 2, 4);
    adicionar_aresta_ponderada(g, 1, 2, 2);
    adicionar_aresta_ponderada(g, 2, 3, 3);
    adicionar_aresta_ponderada(g, 2, 5, 2);
    adicionar_aresta_ponderada(g, 2, 4, 4);
    adicionar_aresta_ponderada(g, 3, 4, 3);
    adicionar_aresta_ponderada(g, 4, 5, 3);

    st_kruskal = kruskal(g, agm_kruskal, &qtd_kruskal, &custo_kruskal);
    st_prim = prim(g, 0, agm_prim, &qtd_prim, &custo_prim);

    imprimir_resultado_agm("Kruskal", st_kruskal, agm_kruskal, qtd_kruskal, custo_kruskal);
    imprimir_resultado_agm("Prim", st_prim, agm_prim, qtd_prim, custo_prim);

    liberar_grafo_ponderado(g);
}
