#include "../agm.h"
#include "../grafo.h"

#include <assert.h>
#include <stdio.h>

static void testar_dsu(void) {
    DSU *d = criar_dsu(5);

    assert(d != NULL);
    assert(find(d, 0) == 0);
    assert(find(d, 4) == 4);
    assert(unir(d, 0, 1) == 1);
    assert(unir(d, 1, 2) == 1);
    assert(unir(d, 0, 2) == 0);
    assert(find(d, 0) == find(d, 2));
    assert(find(d, 5) == -1);

    liberar_dsu(d);
}

static void testar_kruskal_e_prim_conexo(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(6, 0);
    Aresta agm_k[5];
    Aresta agm_p[5];
    size_t qtd_k = 0;
    size_t qtd_p = 0;
    int custo_k = 0;
    int custo_p = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 1, 4));
    assert(adicionar_aresta_ponderada(g, 0, 2, 4));
    assert(adicionar_aresta_ponderada(g, 1, 2, 2));
    assert(adicionar_aresta_ponderada(g, 2, 3, 3));
    assert(adicionar_aresta_ponderada(g, 2, 5, 2));
    assert(adicionar_aresta_ponderada(g, 2, 4, 4));
    assert(adicionar_aresta_ponderada(g, 3, 4, 3));
    assert(adicionar_aresta_ponderada(g, 4, 5, 3));

    assert(kruskal(g, agm_k, &qtd_k, &custo_k) == AGM_OK);
    assert(prim(g, 0, agm_p, &qtd_p, &custo_p) == AGM_OK);
    assert(qtd_k == 5 && qtd_p == 5);
    assert(custo_k == 14 && custo_p == 14);

    liberar_grafo_ponderado(g);
}

static void testar_pesos_negativos_e_lacos(void) {
    GrafoListaPonderado *g = criar_grafo_ponderado(4, 0);
    Aresta agm_k[3];
    Aresta agm_p[3];
    size_t qtd_k = 0;
    size_t qtd_p = 0;
    int custo_k = 0;
    int custo_p = 0;

    assert(g != NULL);
    assert(adicionar_aresta_ponderada(g, 0, 0, -10));
    assert(adicionar_aresta_ponderada(g, 0, 1, -5));
    assert(adicionar_aresta_ponderada(g, 1, 2, 2));
    assert(adicionar_aresta_ponderada(g, 2, 3, -1));
    assert(adicionar_aresta_ponderada(g, 0, 3, 4));

    assert(kruskal(g, agm_k, &qtd_k, &custo_k) == AGM_OK);
    assert(prim(g, 2, agm_p, &qtd_p, &custo_p) == AGM_OK);
    assert(qtd_k == 3 && qtd_p == 3);
    assert(custo_k == -4 && custo_p == -4);

    liberar_grafo_ponderado(g);
}

static void testar_vazio_isolado_e_desconexo(void) {
    GrafoListaPonderado *vazio = criar_grafo_ponderado(0, 0);
    GrafoListaPonderado *isolado = criar_grafo_ponderado(1, 0);
    GrafoListaPonderado *desconexo = criar_grafo_ponderado(4, 0);
    Aresta agm[3];
    size_t qtd = 0;
    int custo = 0;

    assert(vazio != NULL);
    assert(kruskal(vazio, agm, &qtd, &custo) == AGM_INVALIDO);
    assert(prim(vazio, 0, agm, &qtd, &custo) == AGM_INVALIDO);
    liberar_grafo_ponderado(vazio);

    assert(isolado != NULL);
    assert(kruskal(isolado, agm, &qtd, &custo) == AGM_OK);
    assert(qtd == 0 && custo == 0);
    assert(prim(isolado, 0, agm, &qtd, &custo) == AGM_OK);
    assert(qtd == 0 && custo == 0);
    liberar_grafo_ponderado(isolado);

    assert(desconexo != NULL);
    assert(adicionar_aresta_ponderada(desconexo, 0, 1, 3));
    assert(adicionar_aresta_ponderada(desconexo, 2, 3, 5));

    assert(kruskal(desconexo, agm, &qtd, &custo) == AGM_DESCONEXO);
    assert(qtd == 2 && custo == 8);
    assert(prim(desconexo, 0, agm, &qtd, &custo) == AGM_DESCONEXO);
    assert(qtd == 1 && custo == 3);
    liberar_grafo_ponderado(desconexo);
}

static void testar_entradas_invalidas(void) {
    GrafoListaPonderado *dir = criar_grafo_ponderado(3, 1);
    Aresta agm[2];
    size_t qtd = 0;
    int custo = 0;

    assert(dir != NULL);
    assert(kruskal(dir, agm, &qtd, &custo) == AGM_INVALIDO);
    assert(prim(dir, 0, agm, &qtd, &custo) == AGM_INVALIDO);
    assert(prim(NULL, 0, agm, &qtd, &custo) == AGM_INVALIDO);
    liberar_grafo_ponderado(dir);
}

int main(void) {
    testar_dsu();
    testar_kruskal_e_prim_conexo();
    testar_pesos_negativos_e_lacos();
    testar_vazio_isolado_e_desconexo();
    testar_entradas_invalidas();
    printf("Todos os testes da pratica08 (AGM) passaram.\n");
    return 0;
}
