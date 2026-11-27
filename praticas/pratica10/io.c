#include "io.h"
#include "hamiltoniano.h"
#include "tps.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
    LEITURA_OK = 1,
    LEITURA_VAZIA = 0,
    LEITURA_INVALIDA = -1
};

static GrafoMatriz *gerar_instancia_tsp_aleatoria(size_t n, unsigned int semente) {
    GrafoMatriz *g = criar_grafo_matriz(n, 0);
    size_t i;
    size_t j;

    if (g == NULL) {
        return NULL;
    }

    srand(semente);
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            int peso = 1 + (rand() % 50);
            adicionar_aresta_matriz(g, (int)i, (int)j, peso);
        }
    }
    return g;
}

int ler_entrada_pratica10(EntradaPratica10 *entrada) {
    int n = 0;
    int m = 0;
    int ponderado = 0;
    int i;
    int lidos;

    if (entrada == NULL) {
        return LEITURA_INVALIDA;
    }
    entrada->grafo = NULL;
    entrada->ponderado = 0;

    lidos = scanf("%d %d %d", &n, &m, &ponderado);
    if (lidos == EOF) {
        return LEITURA_VAZIA;
    }
    if (lidos != 3 || n < 0 || m < 0 || (ponderado != 0 && ponderado != 1)) {
        return LEITURA_INVALIDA;
    }

    entrada->grafo = criar_grafo_matriz((size_t)n, 0);
    if (entrada->grafo == NULL) {
        return LEITURA_INVALIDA;
    }
    entrada->ponderado = ponderado;

    for (i = 0; i < m; i++) {
        int u;
        int v;
        int peso = 1;
        if (ponderado) {
            if (scanf("%d %d %d", &u, &v, &peso) != 3) {
                liberar_entrada_pratica10(entrada);
                return LEITURA_INVALIDA;
            }
        } else {
            if (scanf("%d %d", &u, &v) != 2) {
                liberar_entrada_pratica10(entrada);
                return LEITURA_INVALIDA;
            }
        }
        if (!adicionar_aresta_matriz(entrada->grafo, u, v, peso)) {
            liberar_entrada_pratica10(entrada);
            return LEITURA_INVALIDA;
        }
    }

    return LEITURA_OK;
}

void liberar_entrada_pratica10(EntradaPratica10 *entrada) {
    if (entrada == NULL) {
        return;
    }
    liberar_grafo_matriz(entrada->grafo);
    entrada->grafo = NULL;
}

void imprimir_sequencia(const char *rotulo, const int *sequencia, size_t n, int fechar_ciclo) {
    size_t i;

    printf("%s: ", rotulo);
    for (i = 0; i < n; i++) {
        printf("%d", sequencia[i]);
        if (i + 1 < n || fechar_ciclo) {
            printf(" -> ");
        }
    }
    if (fechar_ciclo && n > 0) {
        printf("%d", sequencia[0]);
    }
    printf("\n");
}

static void demonstrar_reducao_hamiltoniano(void) {
    GrafoMatriz *g = criar_grafo_matriz(5, 0);
    GrafoMatriz *tsp;
    int caminho[5];
    int ciclo[5];
    int rota[5];
    int custo = 0;

    if (g == NULL) {
        return;
    }

    adicionar_aresta_matriz(g, 0, 1, 1);
    adicionar_aresta_matriz(g, 1, 2, 1);
    adicionar_aresta_matriz(g, 2, 3, 1);
    adicionar_aresta_matriz(g, 3, 4, 1);
    adicionar_aresta_matriz(g, 4, 0, 1);
    adicionar_aresta_matriz(g, 0, 2, 1);

    if (encontrar_caminho_hamiltoniano_bt(g, caminho)) {
        imprimir_sequencia("Caminho Hamiltoniano", caminho, 5, 0);
    }
    if (encontrar_ciclo_hamiltoniano_bt(g, ciclo)) {
        imprimir_sequencia("Ciclo Hamiltoniano", ciclo, 5, 1);
    }

    tsp = grafo_para_tsp(g);
    if (tsp != NULL && tsp_forca_bruta(tsp, 0, rota, &custo)) {
        printf("Reducao grafo->TSP (custo = %d, esperado = 5 se houver ciclo): ", custo);
        imprimir_sequencia("Rota", rota, 5, 1);
    }

    liberar_grafo_matriz(tsp);
    liberar_grafo_matriz(g);
}

void executar_demonstracao_e_experimento(void) {
    static const size_t tamanhos[] = {5, 8, 10, 12};
    size_t qtd_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    size_t idx;

    demonstrar_reducao_hamiltoniano();
    printf("\nExperimento TSP (Forca Bruta vs Vizinho Mais Proximo vs Insercao):\n");
    printf("%-4s | %-10s | %-12s | %-10s | %-12s | %-10s | %-12s\n",
           "n", "Otimo", "Tempo FB(ms)", "VMP", "Tempo VMP", "Insercao", "Tempo Ins");

    for (idx = 0; idx < qtd_tamanhos; idx++) {
        size_t n = tamanhos[idx];
        GrafoMatriz *g = gerar_instancia_tsp_aleatoria(n, (unsigned int)(200 + n));
        int *rota = malloc(n * sizeof(int));
        int custo_fb = 0;
        int custo_vmp = 0;
        int custo_ins = 0;
        clock_t t0;
        clock_t t1;
        double ms_fb;
        double ms_vmp;
        double ms_ins;

        if (g == NULL || rota == NULL) {
            free(rota);
            liberar_grafo_matriz(g);
            return;
        }

        t0 = clock();
        tsp_forca_bruta(g, 0, rota, &custo_fb);
        t1 = clock();
        ms_fb = ((double)(t1 - t0) * 1000.0) / (double)CLOCKS_PER_SEC;

        t0 = clock();
        tsp_vizinho_mais_proximo(g, 0, rota, &custo_vmp);
        t1 = clock();
        ms_vmp = ((double)(t1 - t0) * 1000.0) / (double)CLOCKS_PER_SEC;

        t0 = clock();
        tsp_insercao(g, 0, rota, &custo_ins);
        t1 = clock();
        ms_ins = ((double)(t1 - t0) * 1000.0) / (double)CLOCKS_PER_SEC;

        printf("%-4zu | %-10d | %-12.4f | %-10d | %-12.4f | %-10d | %-12.4f\n",
               n, custo_fb, ms_fb, custo_vmp, ms_vmp, custo_ins, ms_ins);

        free(rota);
        liberar_grafo_matriz(g);
    }
}
