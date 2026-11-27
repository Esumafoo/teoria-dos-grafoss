#include "bellman_ford.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    EntradaBellmanFord entrada;
    int status = ler_entrada_bellman_ford(&entrada);
    int *dist;
    int *pred;
    int resultado;

    if (status == 0) {
        executar_demonstracao_bellman_ford();
        return EXIT_SUCCESS;
    }

    if (status < 0) {
        fprintf(stderr, "Entrada invalida para Bellman-Ford.\n");
        return EXIT_FAILURE;
    }

    if (entrada.grafo->num_vertices == 0) {
        printf("Grafo vazio: nenhum caminho a calcular.\n");
        liberar_entrada_bellman_ford(&entrada);
        return EXIT_SUCCESS;
    }

    dist = malloc(entrada.grafo->num_vertices * sizeof(int));
    pred = malloc(entrada.grafo->num_vertices * sizeof(int));
    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        free(dist);
        free(pred);
        liberar_entrada_bellman_ford(&entrada);
        return EXIT_FAILURE;
    }

    resultado = bellman_ford(entrada.grafo, entrada.origem, dist, pred);
    imprimir_resultado_bellman_ford(&entrada, dist, pred, resultado);

    free(dist);
    free(pred);
    liberar_entrada_bellman_ford(&entrada);
    return EXIT_SUCCESS;
}
