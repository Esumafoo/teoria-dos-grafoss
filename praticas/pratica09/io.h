#ifndef IO_H
#define IO_H

#include "grafo.h"

int ler_grafo_cobertura(GrafoLista **grafo);
void imprimir_cobertura(const char *rotulo, const int *selecionado, size_t n, size_t tamanho);
void executar_demonstracao_e_experimento(void);

#endif
