#include "io.h"
#include "cobertura.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
    LEITURA_OK = 1,
    LEITURA_VAZIA = 0,
    LEITURA_INVALIDA = -1
};

static GrafoLista *gerar_grafo_aleatorio(size_t n, unsigned int semente, double densidade) {
    GrafoLista *g = criar_grafo(n);
    size_t i;
    size_t j;

    if (g == NULL) {
        return NULL;
    }

    srand(semente);
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            double r = (double)rand() / (double)RAND_MAX;
            if (r < densidade) {
                if (!adicionar_aresta(g, (int)i, (int)j)) {
                    liberar_grafo(g);
                    return NULL;
                }
            }
        }
    }
    return g;
}

int ler_grafo_cobertura(GrafoLista **grafo) {
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

    *grafo = criar_grafo((size_t)n);
    if (*grafo == NULL) {
        return LEITURA_INVALIDA;
    }

    for (i = 0; i < m; i++) {
        int u;
        int v;
        if (scanf("%d %d", &u, &v) != 2 || !adicionar_aresta(*grafo, u, v)) {
            liberar_grafo(*grafo);
            *grafo = NULL;
            return LEITURA_INVALIDA;
        }
    }

    return LEITURA_OK;
}

void imprimir_cobertura(const char *rotulo, const int *selecionado, size_t n, size_t tamanho) {
    size_t i;
    int primeiro = 1;

    printf("%s (tamanho = %zu): {", rotulo, tamanho);
    for (i = 0; i < n; i++) {
        if (selecionado[i]) {
            if (!primeiro) {
                printf(", ");
            }
            printf("%zu", i);
            primeiro = 0;
        }
    }
    printf("}\n");
}

void executar_demonstracao_e_experimento(void) {
    static const size_t tamanhos[] = {5, 8, 10, 12, 15};
    size_t qtd_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    size_t idx;

    printf("Experimento de Cobertura de Vertices (Backtracking vs Gulosa):\n");
    printf("%-6s | %-10s | %-12s | %-10s | %-12s\n",
           "n", "Otima", "Tempo BT(ms)", "Gulosa", "Tempo Gul(ms)");

    for (idx = 0; idx < qtd_tamanhos; idx++) {
        size_t n = tamanhos[idx];
        GrafoLista *g = gerar_grafo_aleatorio(n, (unsigned int)(100 + n), 0.45);
        int *sel_otima = malloc(n * sizeof(int));
        int *sel_gulosa = malloc(n * sizeof(int));
        size_t tam_otima = 0;
        size_t tam_gulosa = 0;
        clock_t inicio_bt;
        clock_t fim_bt;
        clock_t inicio_gul;
        clock_t fim_gul;
        double tempo_bt_ms;
        double tempo_gul_ms;

        if (g == NULL || sel_otima == NULL || sel_gulosa == NULL) {
            free(sel_otima);
            free(sel_gulosa);
            liberar_grafo(g);
            return;
        }

        inicio_bt = clock();
        cobertura_minima_bt(g, sel_otima, &tam_otima);
        fim_bt = clock();

        inicio_gul = clock();
        cobertura_gulosa(g, sel_gulosa, &tam_gulosa);
        fim_gul = clock();

        tempo_bt_ms = ((double)(fim_bt - inicio_bt) * 1000.0) / (double)CLOCKS_PER_SEC;
        tempo_gul_ms = ((double)(fim_gul - inicio_gul) * 1000.0) / (double)CLOCKS_PER_SEC;

        printf("%-6zu | %-10zu | %-12.4f | %-10zu | %-12.4f\n",
               n, tam_otima, tempo_bt_ms, tam_gulosa, tempo_gul_ms);

        free(sel_otima);
        free(sel_gulosa);
        liberar_grafo(g);
    }
}
