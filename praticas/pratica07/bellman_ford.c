#include "bellman_ford.h"

#include <stdio.h>
#include <stdlib.h>

static void inicializar_vetores(size_t num_vertices, int origem, int *dist, int *pred) {
    size_t i;

    for (i = 0; i < num_vertices; i++) {
        dist[i] = DISTANCIA_INFINITA;
        pred[i] = SEM_PREDECESSOR;
    }
    dist[origem] = 0;
}

static int tentar_relaxar_aresta(int u, int v, int peso, int *dist, int *pred) {
    long long nova_dist;

    if (dist[u] == DISTANCIA_INFINITA) {
        return 0;
    }

    nova_dist = (long long)dist[u] + (long long)peso;
    if (nova_dist < (long long)INT_MIN) {
        nova_dist = INT_MIN;
    }

    if (nova_dist < (long long)dist[v]) {
        dist[v] = (int)nova_dist;
        pred[v] = u;
        return 1;
    }
    return 0;
}

static int executar_rodada_relaxacao(const GrafoListaPonderado *g, int *dist, int *pred) {
    size_t u;
    int houve_melhoria = 0;

    for (u = 0; u < g->num_vertices; u++) {
        const ArestaLista *aresta = g->adj[u];
        while (aresta != NULL) {
            if (tentar_relaxar_aresta((int)u, aresta->destino, aresta->peso, dist, pred)) {
                houve_melhoria = 1;
            }
            aresta = aresta->prox;
        }
    }

    return houve_melhoria;
}

static int encontrar_vertice_relaxavel(const GrafoListaPonderado *g, const int *dist, int *pred) {
    size_t u;

    for (u = 0; u < g->num_vertices; u++) {
        const ArestaLista *aresta;
        if (dist[u] == DISTANCIA_INFINITA) {
            continue;
        }
        aresta = g->adj[u];
        while (aresta != NULL) {
            long long nova_dist = (long long)dist[u] + (long long)aresta->peso;
            if (nova_dist < (long long)dist[aresta->destino]) {
                if (pred != NULL) {
                    pred[aresta->destino] = (int)u;
                }
                return aresta->destino;
            }
            aresta = aresta->prox;
        }
    }

    return SEM_PREDECESSOR;
}

static void reverter_array(int *dados, size_t tamanho) {
    size_t i;

    for (i = 0; i < tamanho / 2; i++) {
        int temp = dados[i];
        dados[i] = dados[tamanho - 1 - i];
        dados[tamanho - 1 - i] = temp;
    }
}

StatusBellmanFord bellman_ford_status(const GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    size_t iteracao;

    if (g == NULL || g->num_vertices == 0 || dist == NULL || pred == NULL) {
        return BF_STATUS_ENTRADA_INVALIDA;
    }
    if (origem < 0 || (size_t)origem >= g->num_vertices) {
        return BF_STATUS_ENTRADA_INVALIDA;
    }

    inicializar_vetores(g->num_vertices, origem, dist, pred);

    for (iteracao = 1; iteracao < g->num_vertices; iteracao++) {
        if (!executar_rodada_relaxacao(g, dist, pred)) {
            break;
        }
    }

    if (encontrar_vertice_relaxavel(g, dist, pred) != SEM_PREDECESSOR) {
        return BF_STATUS_CICLO_NEGATIVO;
    }

    return BF_STATUS_OK;
}

int bellman_ford(const GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    return bellman_ford_status(g, origem, dist, pred) == BF_STATUS_OK ? 1 : 0;
}

int obter_ciclo_negativo(const GrafoListaPonderado *g, const int *dist, int *pred, int *ciclo, size_t *tamanho) {
    int vertice;
    int atual;
    size_t passo;
    size_t contagem = 0;

    if (g == NULL || g->num_vertices == 0 || dist == NULL || pred == NULL || tamanho == NULL) {
        return 0;
    }

    vertice = encontrar_vertice_relaxavel(g, dist, pred);
    if (vertice == SEM_PREDECESSOR) {
        *tamanho = 0;
        return 0;
    }

    for (passo = 0; passo < g->num_vertices; passo++) {
        vertice = pred[vertice];
        if (vertice == SEM_PREDECESSOR) {
            *tamanho = 0;
            return 0;
        }
    }

    atual = vertice;
    do {
        if (ciclo != NULL) {
            ciclo[contagem] = atual;
        }
        contagem++;
        atual = pred[atual];
    } while (atual != vertice && atual != SEM_PREDECESSOR && contagem <= g->num_vertices);

    if (atual != vertice) {
        *tamanho = 0;
        return 0;
    }

    if (ciclo != NULL) {
        ciclo[contagem] = vertice;
        reverter_array(ciclo, contagem + 1);
    }

    *tamanho = contagem + 1;
    return 1;
}

void reconstruir_ciclo_negativo(const GrafoListaPonderado *g, const int *dist, int *pred) {
    size_t tamanho = 0;
    int *ciclo;
    size_t i;

    if (g == NULL || g->num_vertices == 0) {
        printf("Nenhum ciclo negativo detectado.\n");
        return;
    }

    ciclo = malloc((g->num_vertices + 1) * sizeof(int));
    if (ciclo == NULL) {
        fprintf(stderr, "Erro de memoria ao reconstruir ciclo negativo.\n");
        return;
    }

    if (!obter_ciclo_negativo(g, dist, pred, ciclo, &tamanho)) {
        printf("Nenhum ciclo negativo alcancavel detectado.\n");
        free(ciclo);
        return;
    }

    printf("Ciclo negativo detectado: ");
    for (i = 0; i < tamanho; i++) {
        printf("%d", ciclo[i]);
        if (i + 1 < tamanho) {
            printf(" -> ");
        }
    }
    printf("\n");

    free(ciclo);
}

int obter_caminho_bellman_ford(int origem, int destino, const int *pred, size_t num_vertices, int *caminho, size_t *tamanho) {
    size_t contagem = 0;
    int atual = destino;
    size_t i;

    if (pred == NULL || tamanho == NULL || origem < 0 || destino < 0 ||
        (size_t)origem >= num_vertices || (size_t)destino >= num_vertices) {
        return 0;
    }

    while (atual != origem) {
        if (atual == SEM_PREDECESSOR || contagem >= num_vertices) {
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

void imprimir_caminho_bellman_ford(int origem, int destino, const int *pred, size_t num_vertices) {
    size_t tamanho = 0;
    int *caminho;
    size_t i;

    if (!obter_caminho_bellman_ford(origem, destino, pred, num_vertices, NULL, &tamanho)) {
        printf("Sem caminho de %d ate %d.\n", origem, destino);
        return;
    }

    caminho = malloc(tamanho * sizeof(int));
    if (caminho == NULL) {
        fprintf(stderr, "Erro de memoria ao reconstruir caminho.\n");
        return;
    }

    obter_caminho_bellman_ford(origem, destino, pred, num_vertices, caminho, &tamanho);

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
