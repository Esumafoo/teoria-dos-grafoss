#include "../cobertura.h"
#include "../grafo.h"

#include <assert.h>
#include <stdio.h>

static void testar_vazio_sem_arestas_e_isolado(void) {
    GrafoLista *vazio = criar_grafo(0);
    GrafoLista *isolado = criar_grafo(3);
    int sel[3];
    size_t tamanho = 99;

    assert(vazio != NULL);
    assert(eh_cobertura(vazio, NULL) == 1);
    assert(cobertura_minima_bt(vazio, sel, &tamanho) == 1 && tamanho == 0);
    assert(cobertura_gulosa(vazio, sel, &tamanho) == 1 && tamanho == 0);
    liberar_grafo(vazio);

    assert(isolado != NULL);
    assert(cobertura_minima_bt(isolado, sel, &tamanho) == 1 && tamanho == 0);
    assert(eh_cobertura(isolado, sel) == 1);
    assert(cobertura_gulosa(isolado, sel, &tamanho) == 1 && tamanho == 0);
    assert(eh_cobertura(isolado, sel) == 1);
    liberar_grafo(isolado);
}

static void testar_ciclos_e_estrela(void) {
    GrafoLista *c5 = criar_grafo(5);
    GrafoLista *estrela = criar_grafo(5);
    int sel[5];
    size_t tam = 0;
    size_t i;

    assert(c5 != NULL);
    for (i = 0; i < 5; i++) {
        assert(adicionar_aresta(c5, (int)i, (int)((i + 1) % 5)));
    }
    assert(cobertura_minima_bt(c5, sel, &tam) == 1);
    assert(tam == 3);
    assert(eh_cobertura(c5, sel) == 1);
    assert(cobertura_gulosa(c5, sel, &tam) == 1);
    assert(tam >= 3 && eh_cobertura(c5, sel) == 1);
    liberar_grafo(c5);

    assert(estrela != NULL);
    for (i = 1; i < 5; i++) {
        assert(adicionar_aresta(estrela, 0, (int)i));
    }
    assert(cobertura_minima_bt(estrela, sel, &tam) == 1);
    assert(tam == 1 && sel[0] == 1);
    assert(eh_cobertura(estrela, sel) == 1);
    assert(cobertura_gulosa(estrela, sel, &tam) == 1);
    assert(tam == 1 && sel[0] == 1);
    liberar_grafo(estrela);
}

static void testar_gulosa_subotima_e_bt_otimo(void) {
    GrafoLista *g = criar_grafo(7);
    int sel_otima[7];
    int sel_gulosa[7];
    size_t tam_otima = 0;
    size_t tam_gulosa = 0;

    assert(g != NULL);
    assert(adicionar_aresta(g, 0, 1));
    assert(adicionar_aresta(g, 0, 2));
    assert(adicionar_aresta(g, 0, 3));
    assert(adicionar_aresta(g, 1, 4));
    assert(adicionar_aresta(g, 2, 5));
    assert(adicionar_aresta(g, 3, 6));

    assert(cobertura_minima_bt(g, sel_otima, &tam_otima) == 1);
    assert(eh_cobertura(g, sel_otima) == 1);
    assert(tam_otima == 3);

    assert(cobertura_gulosa(g, sel_gulosa, &tam_gulosa) == 1);
    assert(eh_cobertura(g, sel_gulosa) == 1);
    assert(tam_gulosa == 4);

    liberar_grafo(g);
}

static void testar_desconexo_e_invalidos(void) {
    GrafoLista *g = criar_grafo(5);
    int sel[5] = {0, 0, 0, 0, 0};
    size_t tam = 0;

    assert(g != NULL);
    assert(adicionar_aresta(g, 0, 1));
    assert(adicionar_aresta(g, 2, 3));

    assert(eh_cobertura(g, sel) == 0);
    assert(cobertura_minima_bt(g, sel, &tam) == 1);
    assert(tam == 2 && eh_cobertura(g, sel) == 1);
    assert(sel[4] == 0);

    assert(cobertura_minima_bt(NULL, sel, &tam) == 0);
    assert(cobertura_gulosa(g, NULL, &tam) == 0);
    assert(eh_cobertura(NULL, sel) == 0);

    liberar_grafo(g);
}

int main(void) {
    testar_vazio_sem_arestas_e_isolado();
    testar_ciclos_e_estrela();
    testar_gulosa_subotima_e_bt_otimo();
    testar_desconexo_e_invalidos();
    printf("Todos os testes da pratica09 (Cobertura de Vertices) passaram.\n");
    return 0;
}
