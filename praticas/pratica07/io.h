#ifndef IO_H
#define IO_H

#include "grafo.h"

typedef struct {
    GrafoListaPonderado *grafo;
    int origem;
    int destino;
} EntradaBellmanFord;

int ler_entrada_bellman_ford(EntradaBellmanFord *entrada);
void liberar_entrada_bellman_ford(EntradaBellmanFord *entrada);
void imprimir_resultado_bellman_ford(const EntradaBellmanFord *entrada, const int *dist, int *pred, int status_ok);
void executar_demonstracao_bellman_ford(void);

#endif
