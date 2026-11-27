#include "../bellman_ford.h"
#include "../grafo.h"

#include <assert.h>
#include <stdio.h>

static void testar_pesos_negativos_sem_ciclo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(5, 1);
    int dist[5];
    int pred[5];
    int caminho[5];
    size_t tamanho = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 6));
    assert(adicionar_aresta_ponderada(g, 0, 2, 7));
    assert(adicionar_aresta_ponderada(g, 1, 2, 8));
    assert(adicionar_aresta_ponderada(g, 1, 3, 5));
    assert(adicionar_aresta_ponderada(g, 1, 4, -4));
    assert(adicionar_aresta_ponderada(g, 2, 3, -3));
    assert(adicionar_aresta_ponderada(g, 2, 4, 9));
    assert(adicionar_aresta_ponderada(g, 3, 1, -2));
    assert(adicionar_aresta_ponderada(g, 4, 0, 2));
    assert(adicionar_aresta_ponderada(g, 4, 3, 7));

    assert(bellman_ford(g, 0, dist, pred) == 1);
    assert(dist[0] == 0);
    assert(dist[1] == 2);
    assert(dist[2] == 7);
    assert(dist[3] == 4);
    assert(dist[4] == -2);

    assert(obter_caminho_bellman_ford(0, 4, pred, g->num_vertices, caminho, &tamanho) == 1);
    assert(tamanho == 5);
    assert(caminho[0] == 0 && caminho[1] == 2 && caminho[2] == 3 && caminho[3] == 1 && caminho[4] == 4);

    liberar_grafo_ponderado(g);
}

static void testar_ciclo_negativo_alcancavel(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(4, 1);
    int dist[4];
    int pred[4];
    int ciclo[5];
    size_t tamanho = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 1));
    assert(adicionar_aresta_ponderada(g, 1, 2, -2));
    assert(adicionar_aresta_ponderada(g, 2, 3, -2));
    assert(adicionar_aresta_ponderada(g, 3, 1, -1));

    assert(bellman_ford(g, 0, dist, pred) == 0);
    assert(obter_ciclo_negativo(g, dist, pred, ciclo, &tamanho) == 1);
    assert(tamanho == 4);
    assert(ciclo[0] == ciclo[tamanho - 1]);

    liberar_grafo_ponderado(g);
}

static void testar_ciclo_negativo_nao_alcancavel_e_desconexo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(5, 1);
    int dist[5];
    int pred[5];
    size_t tamanho = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 4));
    assert(adicionar_aresta_ponderada(g, 2, 3, -5));
    assert(adicionar_aresta_ponderada(g, 3, 4, -5));
    assert(adicionar_aresta_ponderada(g, 4, 2, -5));

    assert(bellman_ford(g, 0, dist, pred) == 1);
    assert(dist[0] == 0);
    assert(dist[1] == 4);
    assert(dist[2] == DISTANCIA_INFINITA);
    assert(dist[3] == DISTANCIA_INFINITA);
    assert(dist[4] == DISTANCIA_INFINITA);
    assert(obter_caminho_bellman_ford(0, 2, pred, g->num_vertices, NULL, &tamanho) == 0);

    liberar_grafo_ponderado(g);
}

static void testar_vazio_isolado_e_invalidos(void) {
    GrafoListaPonderado *vazio = criar_grafo_ponderado(0, 1);
    GrafoListaPonderado *isolado = criar_grafo_ponderado(1, 1);
    int dist[1];
    int pred[1];

    assert(vazio != NULL);
    assert(bellman_ford_status(vazio, 0, dist, pred) == BF_STATUS_ENTRADA_INVALIDA);
    liberar_grafo_ponderado(vazio);

    assert(isolado != NULL);
    assert(bellman_ford(isolado, 0, dist, pred) == 1);
    assert(dist[0] == 0);
    assert(pred[0] == SEM_PREDECESSOR);
    assert(bellman_ford_status(isolado, -1, dist, pred) == BF_STATUS_ENTRADA_INVALIDA);
    assert(bellman_ford_status(isolado, 1, dist, pred) == BF_STATUS_ENTRADA_INVALIDA);
    liberar_grafo_ponderado(isolado);
}

static void testar_nao_direcionado_aresta_negativa_gera_ciclo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(2, 0);
    int dist[2];
    int pred[2];
    int ciclo[3];
    size_t tamanho = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, -3));

    assert(bellman_ford(g, 0, dist, pred) == 0);
    assert(obter_ciclo_negativo(g, dist, pred, ciclo, &tamanho) == 1);
    assert(tamanho == 3);
    assert(ciclo[0] == ciclo[2]);

    liberar_grafo_ponderado(g);
}

int main(void) {
    testar_pesos_negativos_sem_ciclo();
    testar_ciclo_negativo_alcancavel();
    testar_ciclo_negativo_nao_alcancavel_e_desconexo();
    testar_vazio_isolado_e_invalidos();
    testar_nao_direcionado_aresta_negativa_gera_ciclo();
    printf("Todos os testes da pratica07 (Bellman-Ford) passaram.\n");
    return 0;
}
