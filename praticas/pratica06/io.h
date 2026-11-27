#ifndef IO_H
#define IO_H

#include "grafo.h"

typedef struct {
    GrafoListaPonderado *grafo;
    int origem;
    int destino;
} EntradaDijkstra;

int ler_entrada_dijkstra(EntradaDijkstra *entrada);
void liberar_entrada_dijkstra(EntradaDijkstra *entrada);
void imprimir_resultado_dijkstra(const EntradaDijkstra *entrada, const int *dist, const int *pred);
void executar_demonstracao_dijkstra(void);

#endif
