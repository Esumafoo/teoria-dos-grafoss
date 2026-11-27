#include "dijkstra.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    EntradaDijkstra entrada;
    int status = ler_entrada_dijkstra(&entrada);
    int *dist;
    int *pred;

    if (status == 0) {
        executar_demonstracao_dijkstra();
        return EXIT_SUCCESS;
    }

    if (status < 0) {
        fprintf(stderr, "Entrada invalida para Dijkstra.\n");
        return EXIT_FAILURE;
    }

    if (entrada.grafo->num_vertices == 0) {
        printf("Grafo vazio: nenhum caminho a calcular.\n");
        liberar_entrada_dijkstra(&entrada);
        return EXIT_SUCCESS;
    }

    dist = malloc(entrada.grafo->num_vertices * sizeof(int));
    pred = malloc(entrada.grafo->num_vertices * sizeof(int));
    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        free(dist);
        free(pred);
        liberar_entrada_dijkstra(&entrada);
        return EXIT_FAILURE;
    }

    if (!dijkstra(entrada.grafo, entrada.origem, dist, pred)) {
        fprintf(stderr, "Falha ao executar Dijkstra (verifique pesos e vertices).\n");
        free(dist);
        free(pred);
        liberar_entrada_dijkstra(&entrada);
        return EXIT_FAILURE;
    }

    imprimir_resultado_dijkstra(&entrada, dist, pred);

    free(dist);
    free(pred);
    liberar_entrada_dijkstra(&entrada);
    return EXIT_SUCCESS;
}
