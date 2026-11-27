#include "hamiltoniano.h"

#include <stdlib.h>

static int buscar_caminho_rec(const GrafoMatriz *g, int atual, size_t profundidade, int *visitado, int *caminho) {
    size_t proximo;

    if (profundidade == g->num_vertices) {
        return 1;
    }

    for (proximo = 0; proximo < g->num_vertices; proximo++) {
        if (!visitado[proximo] && possui_aresta_matriz(g, atual, (int)proximo)) {
            visitado[proximo] = 1;
            caminho[profundidade] = (int)proximo;
            if (buscar_caminho_rec(g, (int)proximo, profundidade + 1, visitado, caminho)) {
                return 1;
            }
            visitado[proximo] = 0;
        }
    }

    return 0;
}

static int buscar_ciclo_rec(const GrafoMatriz *g, int atual, size_t profundidade, int *visitado, int *ciclo) {
    size_t proximo;

    if (profundidade == g->num_vertices) {
        return possui_aresta_matriz(g, atual, ciclo[0]);
    }

    for (proximo = 1; proximo < g->num_vertices; proximo++) {
        if (!visitado[proximo] && possui_aresta_matriz(g, atual, (int)proximo)) {
            visitado[proximo] = 1;
            ciclo[profundidade] = (int)proximo;
            if (buscar_ciclo_rec(g, (int)proximo, profundidade + 1, visitado, ciclo)) {
                return 1;
            }
            visitado[proximo] = 0;
        }
    }

    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const GrafoMatriz *g, int *caminho) {
    int *visitado;
    size_t inicio;

    if (g == NULL || g->num_vertices == 0 || caminho == NULL) {
        return 0;
    }

    visitado = calloc(g->num_vertices, sizeof(int));
    if (visitado == NULL) {
        return 0;
    }

    for (inicio = 0; inicio < g->num_vertices; inicio++) {
        visitado[inicio] = 1;
        caminho[0] = (int)inicio;
        if (buscar_caminho_rec(g, (int)inicio, 1, visitado, caminho)) {
            free(visitado);
            return 1;
        }
        visitado[inicio] = 0;
    }

    free(visitado);
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(const GrafoMatriz *g, int *ciclo) {
    int *visitado;
    int encontrado;

    if (g == NULL || g->num_vertices < 3 || ciclo == NULL) {
        return 0;
    }

    visitado = calloc(g->num_vertices, sizeof(int));
    if (visitado == NULL) {
        return 0;
    }

    visitado[0] = 1;
    ciclo[0] = 0;
    encontrado = buscar_ciclo_rec(g, 0, 1, visitado, ciclo);

    free(visitado);
    return encontrado;
}
