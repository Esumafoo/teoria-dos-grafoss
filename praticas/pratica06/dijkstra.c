#include "dijkstra.h"

#include <stdio.h>
#include <stdlib.h>

static void trocar_nos(MinHeap *heap, int indice_a, int indice_b) {
    NoHeap temporario = heap->dados[indice_a];
    heap->dados[indice_a] = heap->dados[indice_b];
    heap->dados[indice_b] = temporario;

    heap->posicao[heap->dados[indice_a].vertice] = indice_a;
    heap->posicao[heap->dados[indice_b].vertice] = indice_b;
}

static void subir_no_heap(MinHeap *heap, int indice) {
    while (indice > 0) {
        int pai = (indice - 1) / 2;
        if (heap->dados[indice].distancia >= heap->dados[pai].distancia) {
            break;
        }
        trocar_nos(heap, indice, pai);
        indice = pai;
    }
}

static void descer_no_heap(MinHeap *heap, int indice) {
    while (1) {
        int menor = indice;
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;

        if (esquerda < heap->tamanho &&
            heap->dados[esquerda].distancia < heap->dados[menor].distancia) {
            menor = esquerda;
        }
        if (direita < heap->tamanho &&
            heap->dados[direita].distancia < heap->dados[menor].distancia) {
            menor = direita;
        }
        if (menor == indice) {
            break;
        }
        trocar_nos(heap, indice, menor);
        indice = menor;
    }
}

static int grafo_possui_peso_negativo(const GrafoListaPonderado *g) {
    size_t i;

    for (i = 0; i < g->num_vertices; i++) {
        const ArestaLista *aresta = g->adj[i];
        while (aresta != NULL) {
            if (aresta->peso < 0) {
                return 1;
            }
            aresta = aresta->prox;
        }
    }
    return 0;
}

static void inicializar_vetores(size_t num_vertices, int origem, int *dist, int *pred) {
    size_t i;

    for (i = 0; i < num_vertices; i++) {
        dist[i] = DISTANCIA_INFINITA;
        pred[i] = SEM_PREDECESSOR;
    }
    dist[origem] = 0;
}

static void relaxar_vizinhos(const GrafoListaPonderado *g, MinHeap *heap, int u, int *dist, int *pred) {
    const ArestaLista *aresta = g->adj[u];

    while (aresta != NULL) {
        int v = aresta->destino;
        int peso = aresta->peso;

        if (dist[u] != DISTANCIA_INFINITA && dist[u] <= DISTANCIA_INFINITA - peso) {
            int nova_dist = dist[u] + peso;
            if (nova_dist < dist[v]) {
                dist[v] = nova_dist;
                pred[v] = u;
                if (heap->posicao[v] == POSICAO_AUSENTE) {
                    inserir_min_heap(heap, v, nova_dist);
                } else {
                    diminuir_chave(heap, v, nova_dist);
                }
            }
        }
        aresta = aresta->prox;
    }
}

MinHeap *criar_min_heap(int capacidade) {
    MinHeap *heap;
    int i;

    if (capacidade <= 0) {
        return NULL;
    }

    heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }

    heap->dados = malloc((size_t)capacidade * sizeof(NoHeap));
    heap->posicao = malloc((size_t)capacidade * sizeof(int));
    if (heap->dados == NULL || heap->posicao == NULL) {
        free(heap->dados);
        free(heap->posicao);
        free(heap);
        return NULL;
    }

    heap->capacidade = capacidade;
    heap->tamanho = 0;
    for (i = 0; i < capacidade; i++) {
        heap->posicao[i] = POSICAO_AUSENTE;
    }

    return heap;
}

void liberar_min_heap(MinHeap *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->dados);
    free(heap->posicao);
    free(heap);
}

int min_heap_vazio(const MinHeap *heap) {
    if (heap == NULL) {
        return 1;
    }
    return heap->tamanho == 0;
}

int inserir_min_heap(MinHeap *heap, int vertice, int distancia) {
    int indice;

    if (heap == NULL || vertice < 0 || vertice >= heap->capacidade) {
        return 0;
    }
    if (heap->posicao[vertice] != POSICAO_AUSENTE) {
        diminuir_chave(heap, vertice, distancia);
        return 1;
    }
    if (heap->tamanho >= heap->capacidade) {
        return 0;
    }

    indice = heap->tamanho;
    heap->dados[indice].vertice = vertice;
    heap->dados[indice].distancia = distancia;
    heap->posicao[vertice] = indice;
    heap->tamanho++;

    subir_no_heap(heap, indice);
    return 1;
}

NoHeap extrair_minimo(MinHeap *heap) {
    NoHeap minimo;
    NoHeap ultimo;

    if (heap == NULL || heap->tamanho == 0) {
        minimo.vertice = SEM_PREDECESSOR;
        minimo.distancia = DISTANCIA_INFINITA;
        return minimo;
    }

    minimo = heap->dados[0];
    ultimo = heap->dados[heap->tamanho - 1];
    heap->tamanho--;

    heap->posicao[minimo.vertice] = POSICAO_AUSENTE;

    if (heap->tamanho > 0) {
        heap->dados[0] = ultimo;
        heap->posicao[ultimo.vertice] = 0;
        descer_no_heap(heap, 0);
    }

    return minimo;
}

void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia) {
    int indice;

    if (heap == NULL || vertice < 0 || vertice >= heap->capacidade) {
        return;
    }

    indice = heap->posicao[vertice];
    if (indice == POSICAO_AUSENTE || nova_distancia >= heap->dados[indice].distancia) {
        return;
    }

    heap->dados[indice].distancia = nova_distancia;
    subir_no_heap(heap, indice);
}

int dijkstra(const GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    MinHeap *heap;

    if (g == NULL || g->num_vertices == 0 || dist == NULL || pred == NULL) {
        return 0;
    }
    if (origem < 0 || (size_t)origem >= g->num_vertices) {
        return 0;
    }
    if (grafo_possui_peso_negativo(g)) {
        return 0;
    }

    inicializar_vetores(g->num_vertices, origem, dist, pred);

    heap = criar_min_heap((int)g->num_vertices);
    if (heap == NULL) {
        return 0;
    }

    inserir_min_heap(heap, origem, 0);

    while (!min_heap_vazio(heap)) {
        NoHeap atual = extrair_minimo(heap);
        if (atual.distancia > dist[atual.vertice]) {
            continue;
        }
        relaxar_vizinhos(g, heap, atual.vertice, dist, pred);
    }

    liberar_min_heap(heap);
    return 1;
}

int obter_caminho_minimo(int origem, int destino, const int *pred, int *caminho, size_t *tamanho) {
    size_t contagem = 0;
    int atual = destino;
    size_t i;

    if (pred == NULL || tamanho == NULL || origem < 0 || destino < 0) {
        return 0;
    }

    while (atual != origem) {
        if (atual == SEM_PREDECESSOR) {
            *tamanho = 0;
            return 0;
        }
        contagem++;
        atual = pred[atual];
    }
    contagem++;

    if (caminho != NULL) {
        atual = destino;
        for (i = contagem; i > 0; i--) {
            caminho[i - 1] = atual;
            atual = pred[atual];
        }
    }

    *tamanho = contagem;
    return 1;
}

void reconstruir_caminho(int origem, int destino, const int *pred) {
    size_t tamanho = 0;
    int *caminho;
    size_t i;

    if (!obter_caminho_minimo(origem, destino, pred, NULL, &tamanho)) {
        printf("Sem caminho de %d ate %d.\n", origem, destino);
        return;
    }

    caminho = malloc(tamanho * sizeof(int));
    if (caminho == NULL) {
        fprintf(stderr, "Erro de memoria ao reconstruir caminho.\n");
        return;
    }

    obter_caminho_minimo(origem, destino, pred, caminho, &tamanho);

    printf("Caminho (%d -> %d): ", origem, destino);
    for (i = 0; i < tamanho; i++) {
        printf("%d", caminho[i]);
        if (i + 1 < tamanho) {
            printf(" -> ");
        }
    }
    printf("\n");

    free(caminho);
}
