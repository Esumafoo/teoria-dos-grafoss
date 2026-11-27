#include "cobertura.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    const GrafoLista *grafo;
    int *atual;
    int *melhor;
    size_t melhor_tamanho;
} EstadoBacktracking;

static int selecionar_vertice_maior_grau_restante(const GrafoLista *g, const int *selecionado, size_t *melhor_grau) {
    size_t u;
    int escolhido = -1;
    size_t max_grau = 0;

    for (u = 0; u < g->num_vertices; u++) {
        const NoLista *aresta;
        size_t grau = 0;

        if (selecionado[u]) {
            continue;
        }

        aresta = g->adj[u];
        while (aresta != NULL) {
            if (!selecionado[aresta->destino]) {
                grau++;
            }
            aresta = aresta->prox;
        }

        if (grau > max_grau) {
            max_grau = grau;
            escolhido = (int)u;
        }
    }

    if (melhor_grau != NULL) {
        *melhor_grau = max_grau;
    }
    return escolhido;
}

static size_t calcular_cota_inferior_emparelhamento(const GrafoLista *g, const int *selecionado) {
    size_t n = g->num_vertices;
    int *emparelhado = calloc(n, sizeof(int));
    size_t cota = 0;
    size_t u;

    if (emparelhado == NULL) {
        return 0;
    }

    for (u = 0; u < n; u++) {
        const NoLista *aresta;
        if (selecionado[u] || emparelhado[u]) {
            continue;
        }
        aresta = g->adj[u];
        while (aresta != NULL) {
            int v = aresta->destino;
            if (!selecionado[v] && !emparelhado[v] && v != (int)u) {
                emparelhado[u] = 1;
                emparelhado[v] = 1;
                cota++;
                break;
            }
            aresta = aresta->prox;
        }
    }

    free(emparelhado);
    return cota;
}

static void buscar_cobertura_bt(EstadoBacktracking *estado, size_t tamanho_atual) {
    size_t grau_max = 0;
    int u;
    size_t cota_inferior;

    if (tamanho_atual >= estado->melhor_tamanho) {
        return;
    }

    u = selecionar_vertice_maior_grau_restante(estado->grafo, estado->atual, &grau_max);
    if (u < 0 || grau_max == 0) {
        memcpy(estado->melhor, estado->atual, estado->grafo->num_vertices * sizeof(int));
        estado->melhor_tamanho = tamanho_atual;
        return;
    }

    cota_inferior = calcular_cota_inferior_emparelhamento(estado->grafo, estado->atual);
    if (tamanho_atual + cota_inferior >= estado->melhor_tamanho) {
        return;
    }

    estado->atual[u] = 1;
    buscar_cobertura_bt(estado, tamanho_atual + 1);
    estado->atual[u] = 0;

    if (tamanho_atual + grau_max < estado->melhor_tamanho) {
        size_t n = estado->grafo->num_vertices;
        int *marcados = calloc(n, sizeof(int));
        const NoLista *aresta;
        size_t adicionados = 0;
        size_t i;

        if (marcados == NULL) {
            return;
        }

        aresta = estado->grafo->adj[u];
        while (aresta != NULL) {
            int v = aresta->destino;
            if (!estado->atual[v] && !marcados[v]) {
                marcados[v] = 1;
                estado->atual[v] = 1;
                adicionados++;
            }
            aresta = aresta->prox;
        }

        buscar_cobertura_bt(estado, tamanho_atual + adicionados);

        for (i = 0; i < n; i++) {
            if (marcados[i]) {
                estado->atual[i] = 0;
            }
        }
        free(marcados);
    }
}

static size_t incluir_lacos(const GrafoLista *g, int *selecionado) {
    size_t u;
    size_t contagem = 0;

    for (u = 0; u < g->num_vertices; u++) {
        if (sao_adjacentes(g, (int)u, (int)u)) {
            selecionado[u] = 1;
            contagem++;
        }
    }
    return contagem;
}

int eh_cobertura(const GrafoLista *g, const int *selecionado) {
    size_t u;

    if (g == NULL) {
        return 0;
    }
    if (g->num_vertices == 0) {
        return 1;
    }
    if (selecionado == NULL) {
        return 0;
    }

    for (u = 0; u < g->num_vertices; u++) {
        const NoLista *aresta = g->adj[u];
        while (aresta != NULL) {
            if (!selecionado[u] && !selecionado[aresta->destino]) {
                return 0;
            }
            aresta = aresta->prox;
        }
    }
    return 1;
}

int cobertura_gulosa(const GrafoLista *g, int *selecionado, size_t *tamanho) {
    size_t contagem;

    if (g == NULL || tamanho == NULL) {
        return 0;
    }
    if (g->num_vertices == 0) {
        *tamanho = 0;
        return 1;
    }
    if (selecionado == NULL) {
        return 0;
    }

    memset(selecionado, 0, g->num_vertices * sizeof(int));
    contagem = incluir_lacos(g, selecionado);

    while (1) {
        size_t grau_max = 0;
        int u = selecionar_vertice_maior_grau_restante(g, selecionado, &grau_max);
        if (u < 0 || grau_max == 0) {
            break;
        }
        selecionado[u] = 1;
        contagem++;
    }

    *tamanho = contagem;
    return 1;
}

int cobertura_minima_bt(const GrafoLista *g, int *melhor_selecao, size_t *tamanho_minimo) {
    EstadoBacktracking estado;
    size_t tamanho_guloso = 0;
    size_t base_lacos;

    if (g == NULL || tamanho_minimo == NULL) {
        return 0;
    }
    if (g->num_vertices == 0) {
        *tamanho_minimo = 0;
        return 1;
    }
    if (melhor_selecao == NULL) {
        return 0;
    }

    if (!cobertura_gulosa(g, melhor_selecao, &tamanho_guloso)) {
        return 0;
    }

    estado.grafo = g;
    estado.melhor = melhor_selecao;
    estado.melhor_tamanho = tamanho_guloso;
    estado.atual = calloc(g->num_vertices, sizeof(int));
    if (estado.atual == NULL) {
        return 0;
    }

    base_lacos = incluir_lacos(g, estado.atual);
    buscar_cobertura_bt(&estado, base_lacos);

    free(estado.atual);
    *tamanho_minimo = estado.melhor_tamanho;
    return 1;
}
