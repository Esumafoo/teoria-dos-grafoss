#include "hamiltoniano.h"
#include "io.h"
#include "tps.h"

#include <stdio.h>
#include <stdlib.h>

static void processar_grafo_nao_ponderado(const GrafoMatriz *g, int *buffer) {
    GrafoMatriz *tsp = grafo_para_tsp(g);
    int custo = 0;

    if (encontrar_caminho_hamiltoniano_bt(g, buffer)) {
        imprimir_sequencia("Caminho Hamiltoniano", buffer, g->num_vertices, 0);
    } else {
        printf("Sem caminho Hamiltoniano.\n");
    }

    if (encontrar_ciclo_hamiltoniano_bt(g, buffer)) {
        imprimir_sequencia("Ciclo Hamiltoniano", buffer, g->num_vertices, 1);
    } else {
        printf("Sem ciclo Hamiltoniano.\n");
    }

    if (tsp != NULL && tsp_forca_bruta(tsp, 0, buffer, &custo)) {
        printf("TSP via reducao (custo = %d): ", custo);
        imprimir_sequencia("Rota", buffer, g->num_vertices, 1);
    }
    liberar_grafo_matriz(tsp);
}

static void processar_grafo_ponderado(const GrafoMatriz *g, int *buffer) {
    int custo = 0;

    if (tsp_forca_bruta(g, 0, buffer, &custo)) {
        printf("TSP Forca Bruta (custo = %d): ", custo);
        imprimir_sequencia("Rota", buffer, g->num_vertices, 1);
    } else {
        printf("TSP Forca Bruta: sem ciclo valido.\n");
    }

    if (tsp_vizinho_mais_proximo(g, 0, buffer, &custo)) {
        printf("TSP Vizinho Mais Proximo (custo = %d): ", custo);
        imprimir_sequencia("Rota", buffer, g->num_vertices, 1);
    }

    if (tsp_insercao(g, 0, buffer, &custo)) {
        printf("TSP Insercao (custo = %d): ", custo);
        imprimir_sequencia("Rota", buffer, g->num_vertices, 1);
    }
}

int main(void) {
    EntradaPratica10 entrada;
    int status = ler_entrada_pratica10(&entrada);
    int *buffer;

    if (status == 0) {
        executar_demonstracao_e_experimento();
        return EXIT_SUCCESS;
    }

    if (status < 0) {
        fprintf(stderr, "Entrada invalida para Pratica 10.\n");
        return EXIT_FAILURE;
    }

    if (entrada.grafo->num_vertices == 0) {
        printf("Grafo vazio: sem caminho ou ciclo.\n");
        liberar_entrada_pratica10(&entrada);
        return EXIT_SUCCESS;
    }

    buffer = malloc(entrada.grafo->num_vertices * sizeof(int));
    if (buffer == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        liberar_entrada_pratica10(&entrada);
        return EXIT_FAILURE;
    }

    if (entrada.ponderado) {
        processar_grafo_ponderado(entrada.grafo, buffer);
    } else {
        processar_grafo_nao_ponderado(entrada.grafo, buffer);
    }

    free(buffer);
    liberar_entrada_pratica10(&entrada);
    return EXIT_SUCCESS;
}
