#include "../dijkstra.h"
#include "../grafo.h"

#include <assert.h>
#include <stdio.h>

static void testar_min_heap_operacoes(void) {
    MinHeap *heap = criar_min_heap(4);
    NoHeap topo;

    assert(heap != NULL);
    assert(min_heap_vazio(heap));

    assert(inserir_min_heap(heap, 0, 10));
    assert(inserir_min_heap(heap, 1, 30));
    assert(inserir_min_heap(heap, 2, 20));
    assert(inserir_min_heap(heap, 3, 5));

    diminuir_chave(heap, 1, 2);

    topo = extrair_minimo(heap);
    assert(topo.vertice == 1 && topo.distancia == 2);
    topo = extrair_minimo(heap);
    assert(topo.vertice == 3 && topo.distancia == 5);
    topo = extrair_minimo(heap);
    assert(topo.vertice == 0 && topo.distancia == 10);
    topo = extrair_minimo(heap);
    assert(topo.vertice == 2 && topo.distancia == 20);
    assert(min_heap_vazio(heap));

    liberar_min_heap(heap);
}

static void testar_grafo_conexo_e_caminho(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(5, 1);
    int dist[5];
    int pred[5];
    int caminho[5];
    size_t tamanho = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 10));
    assert(adicionar_aresta_ponderada(g, 0, 2, 3));
    assert(adicionar_aresta_ponderada(g, 1, 3, 2));
    assert(adicionar_aresta_ponderada(g, 2, 1, 4));
    assert(adicionar_aresta_ponderada(g, 2, 3, 8));
    assert(adicionar_aresta_ponderada(g, 2, 4, 2));
    assert(adicionar_aresta_ponderada(g, 3, 4, 7));
    assert(adicionar_aresta_ponderada(g, 4, 3, 9));

    assert(dijkstra(g, 0, dist, pred) == 1);
    assert(dist[0] == 0);
    assert(dist[1] == 7);
    assert(dist[2] == 3);
    assert(dist[3] == 9);
    assert(dist[4] == 5);

    assert(obter_caminho_minimo(0, 3, pred, caminho, &tamanho) == 1);
    assert(tamanho == 4);
    assert(caminho[0] == 0 && caminho[1] == 2 && caminho[2] == 1 && caminho[3] == 3);

    liberar_grafo_ponderado(g);
}

static void testar_grafo_vazio_e_isolado(void) {
    GrafoListaPonderado *vazio = criar_grafo_ponderado(0, 0);
    GrafoListaPonderado *isolado = criar_grafo_ponderado(1, 0);
    int dist[1];
    int pred[1];
    int caminho[1];
    size_t tamanho = 0;

    assert(vazio != NULL);
    assert(dijkstra(vazio, 0, dist, pred) == 0);
    liberar_grafo_ponderado(vazio);

    assert(isolado != NULL);
    assert(dijkstra(isolado, 0, dist, pred) == 1);
    assert(dist[0] == 0);
    assert(pred[0] == SEM_PREDECESSOR);
    assert(obter_caminho_minimo(0, 0, pred, caminho, &tamanho) == 1);
    assert(tamanho == 1 && caminho[0] == 0);
    liberar_grafo_ponderado(isolado);
}

static void testar_desconexo_e_ciclos(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(5, 0);
    int dist[5];
    int pred[5];
    size_t tamanho = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 4));
    assert(adicionar_aresta_ponderada(g, 1, 2, 1));
    assert(adicionar_aresta_ponderada(g, 2, 0, 2));
    assert(adicionar_aresta_ponderada(g, 3, 4, 5));

    assert(dijkstra(g, 0, dist, pred) == 1);
    assert(dist[0] == 0);
    assert(dist[1] == 3);
    assert(dist[2] == 2);
    assert(dist[3] == DISTANCIA_INFINITA);
    assert(dist[4] == DISTANCIA_INFINITA);
    assert(obter_caminho_minimo(0, 4, pred, NULL, &tamanho) == 0);

    liberar_grafo_ponderado(g);
}

static void testar_entradas_invalidas_e_peso_negativo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(3, 1);
    int dist[3];
    int pred[3];

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 5));
    assert(adicionar_aresta_ponderada(g, 1, 2, -2));

    assert(dijkstra(g, 0, dist, pred) == 0);
    assert(dijkstra(g, -1, dist, pred) == 0);
    assert(dijkstra(g, 3, dist, pred) == 0);
    assert(dijkstra(NULL, 0, dist, pred) == 0);
    assert(adicionar_aresta_ponderada(g, 0, 5, 1) == 0);

    liberar_grafo_ponderado(g);
}

int main(void) {
    testar_min_heap_operacoes();
    testar_grafo_conexo_e_caminho();
    testar_grafo_vazio_e_isolado();
    testar_desconexo_e_ciclos();
    testar_entradas_invalidas_e_peso_negativo();
    printf("Todos os testes da pratica06 (Dijkstra) passaram.\n");
    return 0;
}
