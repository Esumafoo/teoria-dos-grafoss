#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"

#include <limits.h>
#include <stddef.h>

#define DISTANCIA_INFINITA INT_MAX
#define SEM_PREDECESSOR (-1)
#define POSICAO_AUSENTE (-1)

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade;
    int tamanho;
    int *posicao;
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *heap);
int min_heap_vazio(const MinHeap *heap);
int inserir_min_heap(MinHeap *heap, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia);

int dijkstra(const GrafoListaPonderado *g, int origem, int *dist, int *pred);
int obter_caminho_minimo(int origem, int destino, const int *pred, int *caminho, size_t *tamanho);
void reconstruir_caminho(int origem, int destino, const int *pred);

#endif
