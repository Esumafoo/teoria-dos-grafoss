#include "tps.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const GrafoMatriz *grafo;
    int origem;
    int *rota_atual;
    int *melhor_rota;
    int *visitado;
    int *min_saida;
    long long custo_restante_minimo;
    long long melhor_custo;
    int encontrou;
} EstadoTSP;

static int calcular_custo_ciclo(const GrafoMatriz *g, const int *rota, size_t tamanho, int *custo_saida) {
    long long total = 0;
    size_t i;

    for (i = 0; i < tamanho; i++) {
        int u = rota[i];
        int v = rota[(i + 1) % tamanho];
        if (!possui_aresta_matriz(g, u, v)) {
            return 0;
        }
        total += g->peso[u][v];
    }

    if (total > INT_MAX) {
        return 0;
    }
    *custo_saida = (int)total;
    return 1;
}

static void inicializar_min_saida(const GrafoMatriz *g, int *min_saida, long long *soma_min) {
    size_t u;
    size_t v;

    *soma_min = 0;
    for (u = 0; u < g->num_vertices; u++) {
        int menor = INT_MAX;
        for (v = 0; v < g->num_vertices; v++) {
            if (u != v && possui_aresta_matriz(g, (int)u, (int)v) && g->peso[u][v] < menor) {
                menor = g->peso[u][v];
            }
        }
        min_saida[u] = menor == INT_MAX ? 0 : menor;
        *soma_min += min_saida[u];
    }
}

static void buscar_tsp_rec(EstadoTSP *estado, size_t profundidade, long long custo_atual, long long restante_min) {
    const GrafoMatriz *g = estado->grafo;
    size_t n = g->num_vertices;
    int atual = estado->rota_atual[profundidade - 1];
    size_t proximo;

    if (estado->encontrou && custo_atual + restante_min >= estado->melhor_custo) {
        return;
    }

    if (profundidade == n) {
        if (possui_aresta_matriz(g, atual, estado->origem)) {
            long long custo_total = custo_atual + g->peso[atual][estado->origem];
            if (!estado->encontrou || custo_total < estado->melhor_custo) {
                estado->melhor_custo = custo_total;
                estado->encontrou = 1;
                memcpy(estado->melhor_rota, estado->rota_atual, n * sizeof(int));
            }
        }
        return;
    }

    for (proximo = 0; proximo < n; proximo++) {
        if (!estado->visitado[proximo] && possui_aresta_matriz(g, atual, (int)proximo)) {
            long long novo_custo = custo_atual + g->peso[atual][proximo];
            long long novo_restante = restante_min - estado->min_saida[atual];

            if (!estado->encontrou || novo_custo + novo_restante < estado->melhor_custo) {
                estado->visitado[proximo] = 1;
                estado->rota_atual[profundidade] = (int)proximo;
                buscar_tsp_rec(estado, profundidade + 1, novo_custo, novo_restante);
                estado->visitado[proximo] = 0;
            }
        }
    }
}

int tsp_forca_bruta(const GrafoMatriz *g, int origem, int *rota, int *custo_otimo) {
    size_t n;
    EstadoTSP estado;
    int custo_inicial = 0;
    long long soma_min = 0;

    if (g == NULL || g->num_vertices == 0 || rota == NULL || custo_otimo == NULL) {
        return 0;
    }
    n = g->num_vertices;
    if (origem < 0 || (size_t)origem >= n) {
        return 0;
    }
    if (n == 1) {
        rota[0] = origem;
        *custo_otimo = 0;
        return 1;
    }

    estado.grafo = g;
    estado.origem = origem;
    estado.rota_atual = malloc(n * sizeof(int));
    estado.melhor_rota = rota;
    estado.visitado = calloc(n, sizeof(int));
    estado.min_saida = malloc(n * sizeof(int));
    if (estado.rota_atual == NULL || estado.visitado == NULL || estado.min_saida == NULL) {
        free(estado.rota_atual);
        free(estado.visitado);
        free(estado.min_saida);
        return 0;
    }

    estado.encontrou = 0;
    estado.melhor_custo = LLONG_MAX;
    if (tsp_vizinho_mais_proximo(g, origem, rota, &custo_inicial)) {
        estado.encontrou = 1;
        estado.melhor_custo = custo_inicial;
    }

    inicializar_min_saida(g, estado.min_saida, &soma_min);
    estado.visitado[origem] = 1;
    estado.rota_atual[0] = origem;
    buscar_tsp_rec(&estado, 1, 0, soma_min);

    free(estado.rota_atual);
    free(estado.visitado);
    free(estado.min_saida);

    if (!estado.encontrou || estado.melhor_custo > INT_MAX) {
        return 0;
    }

    *custo_otimo = (int)estado.melhor_custo;
    return 1;
}

int tsp_vizinho_mais_proximo(const GrafoMatriz *g, int origem, int *rota, int *custo_total) {
    size_t n;
    int *visitado;
    size_t passo;
    int atual;

    if (g == NULL || g->num_vertices == 0 || rota == NULL || custo_total == NULL) {
        return 0;
    }
    n = g->num_vertices;
    if (origem < 0 || (size_t)origem >= n) {
        return 0;
    }
    if (n == 1) {
        rota[0] = origem;
        *custo_total = 0;
        return 1;
    }

    visitado = calloc(n, sizeof(int));
    if (visitado == NULL) {
        return 0;
    }

    atual = origem;
    visitado[origem] = 1;
    rota[0] = origem;

    for (passo = 1; passo < n; passo++) {
        int melhor_vizinho = -1;
        int menor_peso = INT_MAX;
        size_t candidato;

        for (candidato = 0; candidato < n; candidato++) {
            if (!visitado[candidato] && possui_aresta_matriz(g, atual, (int)candidato)) {
                int w = g->peso[atual][candidato];
                if (melhor_vizinho == -1 || w < menor_peso) {
                    menor_peso = w;
                    melhor_vizinho = (int)candidato;
                }
            }
        }

        if (melhor_vizinho == -1) {
            free(visitado);
            return 0;
        }

        visitado[melhor_vizinho] = 1;
        rota[passo] = melhor_vizinho;
        atual = melhor_vizinho;
    }

    free(visitado);
    return calcular_custo_ciclo(g, rota, n, custo_total);
}

static int encontrar_proximo_para_insercao(const GrafoMatriz *g, const int *na_rota, size_t tamanho_rota, const int *rota) {
    int melhor_candidato = -1;
    int menor_distancia = INT_MAX;
    size_t v;
    size_t i;

    for (v = 0; v < g->num_vertices; v++) {
        if (na_rota[v]) {
            continue;
        }
        for (i = 0; i < tamanho_rota; i++) {
            int u = rota[i];
            if (possui_aresta_matriz(g, u, (int)v)) {
                int dist = g->peso[u][v];
                if (melhor_candidato == -1 || dist < menor_distancia) {
                    menor_distancia = dist;
                    melhor_candidato = (int)v;
                }
            }
        }
    }

    return melhor_candidato;
}

static size_t encontrar_melhor_posicao_insercao(const GrafoMatriz *g, const int *rota, size_t tamanho_rota, int vertice, int *valido) {
    size_t melhor_pos = 0;
    long long menor_aumento = LLONG_MAX;
    size_t i;

    *valido = 0;
    for (i = 0; i < tamanho_rota; i++) {
        int u = rota[i];
        int w = rota[(i + 1) % tamanho_rota];

        if (possui_aresta_matriz(g, u, vertice) &&
            possui_aresta_matriz(g, vertice, w) &&
            possui_aresta_matriz(g, u, w)) {
            long long aumento = (long long)g->peso[u][vertice] +
                                (long long)g->peso[vertice][w] -
                                (long long)g->peso[u][w];
            if (!*valido || aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_pos = i + 1;
                *valido = 1;
            }
        }
    }

    return melhor_pos;
}

int tsp_insercao(const GrafoMatriz *g, int origem, int *rota, int *custo_total) {
    size_t n;
    int *na_rota;
    size_t tamanho_rota = 1;
    int segundo = -1;
    int menor_peso = INT_MAX;
    size_t v;

    if (g == NULL || g->num_vertices == 0 || rota == NULL || custo_total == NULL) {
        return 0;
    }
    n = g->num_vertices;
    if (origem < 0 || (size_t)origem >= n) {
        return 0;
    }
    if (n == 1) {
        rota[0] = origem;
        *custo_total = 0;
        return 1;
    }

    na_rota = calloc(n, sizeof(int));
    if (na_rota == NULL) {
        return 0;
    }

    rota[0] = origem;
    na_rota[origem] = 1;

    for (v = 0; v < n; v++) {
        if (!na_rota[v] && possui_aresta_matriz(g, origem, (int)v) &&
            possui_aresta_matriz(g, (int)v, origem)) {
            if (segundo == -1 || g->peso[origem][v] < menor_peso) {
                menor_peso = g->peso[origem][v];
                segundo = (int)v;
            }
        }
    }

    if (segundo == -1) {
        free(na_rota);
        return 0;
    }

    rota[1] = segundo;
    na_rota[segundo] = 1;
    tamanho_rota = 2;

    while (tamanho_rota < n) {
        int candidato = encontrar_proximo_para_insercao(g, na_rota, tamanho_rota, rota);
        int valido = 0;
        size_t pos;
        size_t k;

        if (candidato == -1) {
            free(na_rota);
            return 0;
        }

        pos = encontrar_melhor_posicao_insercao(g, rota, tamanho_rota, candidato, &valido);
        if (!valido) {
            free(na_rota);
            return 0;
        }

        for (k = tamanho_rota; k > pos; k--) {
            rota[k] = rota[k - 1];
        }
        rota[pos] = candidato;
        na_rota[candidato] = 1;
        tamanho_rota++;
    }

    free(na_rota);
    return calcular_custo_ciclo(g, rota, n, custo_total);
}

GrafoMatriz *grafo_para_tsp(const GrafoMatriz *g) {
    GrafoMatriz *tsp;
    size_t i;
    size_t j;

    if (g == NULL) {
        return NULL;
    }

    tsp = criar_grafo_matriz(g->num_vertices, g->direcionado);
    if (tsp == NULL) {
        return NULL;
    }

    for (i = 0; i < g->num_vertices; i++) {
        for (j = 0; j < g->num_vertices; j++) {
            if (i == j) {
                continue;
            }
            tsp->peso[i][j] = possui_aresta_matriz(g, (int)i, (int)j) ? 1 : 2;
        }
    }

    return tsp;
}
