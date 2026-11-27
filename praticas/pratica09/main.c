#include "cobertura.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    GrafoLista *grafo = NULL;
    int status = ler_grafo_cobertura(&grafo);
    int *sel_otima;
    int *sel_gulosa;
    size_t tam_otima = 0;
    size_t tam_gulosa = 0;

    if (status == 0) {
        executar_demonstracao_e_experimento();
        return EXIT_SUCCESS;
    }

    if (status < 0) {
        fprintf(stderr, "Entrada invalida para Cobertura de Vertices.\n");
        return EXIT_FAILURE;
    }

    if (grafo->num_vertices == 0) {
        printf("Grafo vazio: cobertura minima de tamanho 0.\n");
        liberar_grafo(grafo);
        return EXIT_SUCCESS;
    }

    sel_otima = malloc(grafo->num_vertices * sizeof(int));
    sel_gulosa = malloc(grafo->num_vertices * sizeof(int));
    if (sel_otima == NULL || sel_gulosa == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        free(sel_otima);
        free(sel_gulosa);
        liberar_grafo(grafo);
        return EXIT_FAILURE;
    }

    if (!cobertura_minima_bt(grafo, sel_otima, &tam_otima) ||
        !cobertura_gulosa(grafo, sel_gulosa, &tam_gulosa)) {
        fprintf(stderr, "Falha ao calcular cobertura de vertices.\n");
        free(sel_otima);
        free(sel_gulosa);
        liberar_grafo(grafo);
        return EXIT_FAILURE;
    }

    imprimir_cobertura("Cobertura minima (Backtracking)", sel_otima, grafo->num_vertices, tam_otima);
    imprimir_cobertura("Cobertura aproximada (Gulosa)", sel_gulosa, grafo->num_vertices, tam_gulosa);

    free(sel_otima);
    free(sel_gulosa);
    liberar_grafo(grafo);
    return EXIT_SUCCESS;
}
