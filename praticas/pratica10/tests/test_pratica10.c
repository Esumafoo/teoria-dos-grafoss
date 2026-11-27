#include "../grafo.h"
#include "../hamiltoniano.h"
#include "../tps.h"

#include <assert.h>
#include <stdio.h>

static void testar_hamiltoniano_com_e_sem_solucao(void) {
    GrafoMatriz *caminho_sem_ciclo = criar_grafo_matriz(4, 0);
    GrafoMatriz *estrela_sem_caminho = criar_grafo_matriz(4, 0);
    GrafoMatriz *ciclo_completo = criar_grafo_matriz(4, 0);
    int seq[4];
    size_t i;

    assert(caminho_sem_ciclo != NULL);
    adicionar_aresta_matriz(caminho_sem_ciclo, 0, 1, 1);
    adicionar_aresta_matriz(caminho_sem_ciclo, 1, 2, 1);
    adicionar_aresta_matriz(caminho_sem_ciclo, 2, 3, 1);
    assert(encontrar_caminho_hamiltoniano_bt(caminho_sem_ciclo, seq) == 1);
    assert(encontrar_ciclo_hamiltoniano_bt(caminho_sem_ciclo, seq) == 0);
    liberar_grafo_matriz(caminho_sem_ciclo);

    assert(estrela_sem_caminho != NULL);
    adicionar_aresta_matriz(estrela_sem_caminho, 0, 1, 1);
    adicionar_aresta_matriz(estrela_sem_caminho, 0, 2, 1);
    adicionar_aresta_matriz(estrela_sem_caminho, 0, 3, 1);
    assert(encontrar_caminho_hamiltoniano_bt(estrela_sem_caminho, seq) == 0);
    assert(encontrar_ciclo_hamiltoniano_bt(estrela_sem_caminho, seq) == 0);
    liberar_grafo_matriz(estrela_sem_caminho);

    assert(ciclo_completo != NULL);
    for (i = 0; i < 4; i++) {
        adicionar_aresta_matriz(ciclo_completo, (int)i, (int)((i + 1) % 4), 1);
    }
    assert(encontrar_caminho_hamiltoniano_bt(ciclo_completo, seq) == 1);
    assert(encontrar_ciclo_hamiltoniano_bt(ciclo_completo, seq) == 1);
    liberar_grafo_matriz(ciclo_completo);
}

static void testar_vazio_isolado_e_desconexo(void) {
    GrafoMatriz *vazio = criar_grafo_matriz(0, 0);
    GrafoMatriz *isolado = criar_grafo_matriz(1, 0);
    GrafoMatriz *desconexo = criar_grafo_matriz(4, 0);
    int seq[4];
    int custo = 0;

    assert(vazio != NULL);
    assert(encontrar_caminho_hamiltoniano_bt(vazio, seq) == 0);
    assert(encontrar_ciclo_hamiltoniano_bt(vazio, seq) == 0);
    assert(tsp_forca_bruta(vazio, 0, seq, &custo) == 0);
    liberar_grafo_matriz(vazio);

    assert(isolado != NULL);
    assert(encontrar_caminho_hamiltoniano_bt(isolado, seq) == 1 && seq[0] == 0);
    assert(encontrar_ciclo_hamiltoniano_bt(isolado, seq) == 0);
    assert(tsp_forca_bruta(isolado, 0, seq, &custo) == 1 && custo == 0);
    assert(tsp_vizinho_mais_proximo(isolado, 0, seq, &custo) == 1 && custo == 0);
    assert(tsp_insercao(isolado, 0, seq, &custo) == 1 && custo == 0);
    liberar_grafo_matriz(isolado);

    assert(desconexo != NULL);
    adicionar_aresta_matriz(desconexo, 0, 1, 2);
    adicionar_aresta_matriz(desconexo, 2, 3, 3);
    assert(encontrar_caminho_hamiltoniano_bt(desconexo, seq) == 0);
    assert(tsp_forca_bruta(desconexo, 0, seq, &custo) == 0);
    assert(tsp_vizinho_mais_proximo(desconexo, 0, seq, &custo) == 0);
    assert(tsp_insercao(desconexo, 0, seq, &custo) == 0);
    liberar_grafo_matriz(desconexo);
}

static void testar_tsp_exato_e_heuristicas(void) {
    GrafoMatriz *g = criar_grafo_matriz(4, 0);
    int rota[4];
    int custo_fb = 0;
    int custo_vmp = 0;
    int custo_ins = 0;

    assert(g != NULL);
    adicionar_aresta_matriz(g, 0, 1, 10);
    adicionar_aresta_matriz(g, 0, 2, 15);
    adicionar_aresta_matriz(g, 0, 3, 20);
    adicionar_aresta_matriz(g, 1, 2, 35);
    adicionar_aresta_matriz(g, 1, 3, 25);
    adicionar_aresta_matriz(g, 2, 3, 30);

    assert(tsp_forca_bruta(g, 0, rota, &custo_fb) == 1);
    assert(custo_fb == 80);

    assert(tsp_vizinho_mais_proximo(g, 0, rota, &custo_vmp) == 1);
    assert(custo_vmp >= custo_fb);

    assert(tsp_insercao(g, 0, rota, &custo_ins) == 1);
    assert(custo_ins >= custo_fb);

    liberar_grafo_matriz(g);
}

static void testar_reducao_grafo_para_tsp(void) {
    GrafoMatriz *com_ciclo = criar_grafo_matriz(4, 0);
    GrafoMatriz *sem_ciclo = criar_grafo_matriz(4, 0);
    GrafoMatriz *tsp_com;
    GrafoMatriz *tsp_sem;
    int rota[4];
    int custo = 0;
    size_t i;

    assert(com_ciclo != NULL && sem_ciclo != NULL);
    for (i = 0; i < 4; i++) {
        adicionar_aresta_matriz(com_ciclo, (int)i, (int)((i + 1) % 4), 1);
    }

    adicionar_aresta_matriz(sem_ciclo, 0, 1, 1);
    adicionar_aresta_matriz(sem_ciclo, 1, 2, 1);
    adicionar_aresta_matriz(sem_ciclo, 2, 3, 1);

    tsp_com = grafo_para_tsp(com_ciclo);
    tsp_sem = grafo_para_tsp(sem_ciclo);
    assert(tsp_com != NULL && tsp_sem != NULL);

    assert(tsp_forca_bruta(tsp_com, 0, rota, &custo) == 1 && custo == 4);
    assert(tsp_forca_bruta(tsp_sem, 0, rota, &custo) == 1 && custo > 4);

    liberar_grafo_matriz(tsp_com);
    liberar_grafo_matriz(tsp_sem);
    liberar_grafo_matriz(com_ciclo);
    liberar_grafo_matriz(sem_ciclo);
}

int main(void) {
    testar_hamiltoniano_com_e_sem_solucao();
    testar_vazio_isolado_e_desconexo();
    testar_tsp_exato_e_heuristicas();
    testar_reducao_grafo_para_tsp();
    printf("Todos os testes da pratica10 (Hamiltoniano e TSP) passaram.\n");
    return 0;
}
