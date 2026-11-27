#include "agm.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    GrafoListaPonderado *grafo = NULL;
    int status_leitura = ler_grafo_agm(&grafo);
    Aresta *agm_kruskal;
    Aresta *agm_prim;
    size_t qtd_kruskal = 0;
    size_t qtd_prim = 0;
    int custo_kruskal = 0;
    int custo_prim = 0;
    size_t max_arestas;

    if (status_leitura == 0) {
        executar_demonstracao_agm();
        return EXIT_SUCCESS;
    }

    if (status_leitura < 0) {
        fprintf(stderr, "Entrada invalida para AGM.\n");
        return EXIT_FAILURE;
    }

    if (grafo->num_vertices == 0) {
        printf("Grafo vazio: nenhuma arvore geradora.\n");
        liberar_grafo_ponderado(grafo);
        return EXIT_SUCCESS;
    }

    max_arestas = grafo->num_vertices > 1 ? grafo->num_vertices - 1 : 1;
    agm_kruskal = malloc(max_arestas * sizeof(Aresta));
    agm_prim = malloc(max_arestas * sizeof(Aresta));
    if (agm_kruskal == NULL || agm_prim == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        free(agm_kruskal);
        free(agm_prim);
        liberar_grafo_ponderado(grafo);
        return EXIT_FAILURE;
    }

    imprimir_resultado_agm("Kruskal",
                           kruskal(grafo, agm_kruskal, &qtd_kruskal, &custo_kruskal),
                           agm_kruskal, qtd_kruskal, custo_kruskal);
    imprimir_resultado_agm("Prim",
                           prim(grafo, 0, agm_prim, &qtd_prim, &custo_prim),
                           agm_prim, qtd_prim, custo_prim);

    free(agm_kruskal);
    free(agm_prim);
    liberar_grafo_ponderado(grafo);
    return EXIT_SUCCESS;
}
