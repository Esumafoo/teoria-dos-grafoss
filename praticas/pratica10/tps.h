#ifndef TPS_H
#define TPS_H

#include "grafo.h"

int tsp_forca_bruta(const GrafoMatriz *g, int origem, int *rota, int *custo_otimo);
int tsp_vizinho_mais_proximo(const GrafoMatriz *g, int origem, int *rota, int *custo_total);
int tsp_insercao(const GrafoMatriz *g, int origem, int *rota, int *custo_total);
GrafoMatriz *grafo_para_tsp(const GrafoMatriz *g);

#endif
