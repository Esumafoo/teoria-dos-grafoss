#include "agm.h"

#include <limits.h>
#include <stdlib.h>

#define SEM_PAI (-1)
#define POSICAO_AUSENTE (-1)

typedef struct {
    int vertice;
    int chave;
} NoHeapPrim;

typedef struct {
    NoHeapPrim *dados;
    int capacidade;
    int tamanho;
    int *posicao;
} MinHeapPrim;

static void trocar_nos_prim(MinHeapPrim *heap, int indice_a, int indice_b) {
    NoHeapPrim temp = heap->dados[indice_a];
    heap->dados[indice_a] = heap->dados[indice_b];
    heap->dados[indice_b] = temp;

    heap->posicao[heap->dados[indice_a].vertice] = indice_a;
    heap->posicao[heap->dados[indice_b].vertice] = indice_b;
}

static void subir_prim(MinHeapPrim *heap, int indice) {
    while (indice > 0) {
        int pai = (indice - 1) / 2;
        if (heap->dados[indice].chave >= heap->dados[pai].chave) {
            break;
        }
        trocar_nos_prim(heap, indice, pai);
        indice = pai;
    }
}

static void descer_prim(MinHeapPrim *heap, int indice) {
    while (1) {
        int menor = indice;
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;

        if (esquerda < heap->tamanho &&
            heap->dados[esquerda].chave < heap->dados[menor].chave) {
            menor = esquerda;
        }
        if (direita < heap->tamanho &&
            heap->dados[direita].chave < heap->dados[menor].chave) {
            menor = direita;
        }
        if (menor == indice) {
            break;
        }
        trocar_nos_prim(heap, indice, menor);
        indice = menor;
    }
}

static MinHeapPrim *criar_heap_prim(int capacidade) {
    MinHeapPrim *heap;
    int i;

    if (capacidade <= 0) {
        return NULL;
    }

    heap = malloc(sizeof(MinHeapPrim));
    if (heap == NULL) {
        return NULL;
    }

    heap->dados = malloc((size_t)capacidade * sizeof(NoHeapPrim));
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

static void liberar_heap_prim(MinHeapPrim *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->dados);
    free(heap->posicao);
    free(heap);
}

static void inserir_ou_diminuir_prim(MinHeapPrim *heap, int vertice, int nova_chave) {
    int indice = heap->posicao[vertice];

    if (indice == POSICAO_AUSENTE) {
        indice = heap->tamanho;
        heap->dados[indice].vertice = vertice;
        heap->dados[indice].chave = nova_chave;
        heap->posicao[vertice] = indice;
        heap->tamanho++;
        subir_prim(heap, indice);
        return;
    }

    if (nova_chave < heap->dados[indice].chave) {
        heap->dados[indice].chave = nova_chave;
        subir_prim(heap, indice);
    }
}

static NoHeapPrim extrair_minimo_prim(MinHeapPrim *heap) {
    NoHeapPrim minimo = heap->dados[0];
    NoHeapPrim ultimo = heap->dados[heap->tamanho - 1];

    heap->tamanho--;
    heap->posicao[minimo.vertice] = POSICAO_AUSENTE;

    if (heap->tamanho > 0) {
        heap->dados[0] = ultimo;
        heap->posicao[ultimo.vertice] = 0;
        descer_prim(heap, 0);
    }
    return minimo;
}

static int comparar_arestas(const void *ptr_a, const void *ptr_b) {
    const Aresta *a = (const Aresta *)ptr_a;
    const Aresta *b = (const Aresta *)ptr_b;

    if (a->peso != b->peso) {
        return (a->peso > b->peso) - (a->peso < b->peso);
    }
    if (a->u != b->u) {
        return (a->u > b->u) - (a->u < b->u);
    }
    return (a->v > b->v) - (a->v < b->v);
}

static size_t contar_arestas_nao_direcionadas(const GrafoListaPonderado *g) {
    size_t u;
    size_t total = 0;

    for (u = 0; u < g->num_vertices; u++) {
        const ArestaLista *aresta = g->adj[u];
        while (aresta != NULL) {
            if ((int)u < aresta->destino) {
                total++;
            }
            aresta = aresta->prox;
        }
    }
    return total;
}

static void coletar_arestas_nao_direcionadas(const GrafoListaPonderado *g, Aresta *arestas) {
    size_t u;
    size_t idx = 0;

    for (u = 0; u < g->num_vertices; u++) {
        const ArestaLista *aresta = g->adj[u];
        while (aresta != NULL) {
            if ((int)u < aresta->destino) {
                arestas[idx].u = (int)u;
                arestas[idx].v = aresta->destino;
                arestas[idx].peso = aresta->peso;
                idx++;
            }
            aresta = aresta->prox;
        }
    }
}

DSU *criar_dsu(int n) {
    DSU *d;
    int i;

    if (n <= 0) {
        return NULL;
    }

    d = malloc(sizeof(DSU));
    if (d == NULL) {
        return NULL;
    }

    d->pai = malloc((size_t)n * sizeof(int));
    d->rank = calloc((size_t)n, sizeof(int));
    if (d->pai == NULL || d->rank == NULL) {
        free(d->pai);
        free(d->rank);
        free(d);
        return NULL;
    }

    d->n = n;
    for (i = 0; i < n; i++) {
        d->pai[i] = i;
    }
    return d;
}

void liberar_dsu(DSU *d) {
    if (d == NULL) {
        return;
    }
    free(d->pai);
    free(d->rank);
    free(d);
}

int find(DSU *d, int x) {
    if (d == NULL || x < 0 || x >= d->n) {
        return -1;
    }
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]);
    }
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x < 0 || raiz_y < 0 || raiz_x == raiz_y) {
        return 0;
    }

    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }
    return 1;
}

StatusAGM kruskal(const GrafoListaPonderado *g, Aresta *agm, size_t *num_arestas_agm, int *custo_total) {
    size_t total_arestas;
    Aresta *arestas = NULL;
    DSU *dsu;
    size_t i;
    size_t selecionadas = 0;
    int custo = 0;

    if (g == NULL || g->num_vertices == 0 || g->direcionado ||
        num_arestas_agm == NULL || custo_total == NULL) {
        if (num_arestas_agm != NULL) {
            *num_arestas_agm = 0;
        }
        if (custo_total != NULL) {
            *custo_total = 0;
        }
        return AGM_INVALIDO;
    }

    *num_arestas_agm = 0;
    *custo_total = 0;

    if (g->num_vertices == 1) {
        return AGM_OK;
    }

    total_arestas = contar_arestas_nao_direcionadas(g);
    if (total_arestas == 0) {
        return AGM_DESCONEXO;
    }

    arestas = malloc(total_arestas * sizeof(Aresta));
    dsu = criar_dsu((int)g->num_vertices);
    if (arestas == NULL || dsu == NULL) {
        free(arestas);
        liberar_dsu(dsu);
        return AGM_INVALIDO;
    }

    coletar_arestas_nao_direcionadas(g, arestas);
    qsort(arestas, total_arestas, sizeof(Aresta), comparar_arestas);

    for (i = 0; i < total_arestas && selecionadas < g->num_vertices - 1; i++) {
        if (unir(dsu, arestas[i].u, arestas[i].v)) {
            if (agm != NULL) {
                agm[selecionadas] = arestas[i];
            }
            custo += arestas[i].peso;
            selecionadas++;
        }
    }

    free(arestas);
    liberar_dsu(dsu);

    *num_arestas_agm = selecionadas;
    *custo_total = custo;

    return selecionadas == g->num_vertices - 1 ? AGM_OK : AGM_DESCONEXO;
}

StatusAGM prim(const GrafoListaPonderado *g, int origem, Aresta *agm, size_t *num_arestas_agm, int *custo_total) {
    size_t n;
    int *chave;
    int *pai;
    int *na_agm;
    MinHeapPrim *heap;
    size_t i;
    size_t visitados = 0;
    size_t selecionadas = 0;
    int custo = 0;

    if (g == NULL || g->num_vertices == 0 || g->direcionado ||
        origem < 0 || (size_t)origem >= g->num_vertices ||
        num_arestas_agm == NULL || custo_total == NULL) {
        if (num_arestas_agm != NULL) {
            *num_arestas_agm = 0;
        }
        if (custo_total != NULL) {
            *custo_total = 0;
        }
        return AGM_INVALIDO;
    }

    n = g->num_vertices;
    *num_arestas_agm = 0;
    *custo_total = 0;

    if (n == 1) {
        return AGM_OK;
    }

    chave = malloc(n * sizeof(int));
    pai = malloc(n * sizeof(int));
    na_agm = calloc(n, sizeof(int));
    heap = criar_heap_prim((int)n);
    if (chave == NULL || pai == NULL || na_agm == NULL || heap == NULL) {
        free(chave);
        free(pai);
        free(na_agm);
        liberar_heap_prim(heap);
        return AGM_INVALIDO;
    }

    for (i = 0; i < n; i++) {
        chave[i] = INT_MAX;
        pai[i] = SEM_PAI;
    }

    chave[origem] = 0;
    inserir_ou_diminuir_prim(heap, origem, 0);

    while (heap->tamanho > 0) {
        NoHeapPrim atual = extrair_minimo_prim(heap);
        int u = atual.vertice;
        const ArestaLista *aresta;

        if (na_agm[u]) {
            continue;
        }
        na_agm[u] = 1;
        visitados++;

        if (pai[u] != SEM_PAI) {
            if (agm != NULL) {
                agm[selecionadas].u = pai[u];
                agm[selecionadas].v = u;
                agm[selecionadas].peso = atual.chave;
            }
            custo += atual.chave;
            selecionadas++;
        }

        aresta = g->adj[u];
        while (aresta != NULL) {
            int v = aresta->destino;
            if (!na_agm[v] && aresta->peso < chave[v]) {
                chave[v] = aresta->peso;
                pai[v] = u;
                inserir_ou_diminuir_prim(heap, v, aresta->peso);
            }
            aresta = aresta->prox;
        }
    }

    free(chave);
    free(pai);
    free(na_agm);
    liberar_heap_prim(heap);

    *num_arestas_agm = selecionadas;
    *custo_total = custo;

    return visitados == n ? AGM_OK : AGM_DESCONEXO;
}
